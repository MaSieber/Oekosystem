// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "NewWorldDiscovery.h"
#include "NewWorldDiscoveryCharacter.h"
#include "WorldDiscoveryPlayerController.h"
#include "WorldDiscoveryPlayerState.h"
#include "DrawDebugHelpers.h"
#include "HelperClass.h"

#include "MagneticBox/BaseMagnetic.h"
#include "MagneticBox/MagneticBox.h"
#include "MagneticBox/MagneticEnergyProvider.h"
#include "MagneticBox/MagneticEnergyTransfer.h"
#include "MagneticBox/MagneticShield.h"

#include "PlayerMagnet/Player360Degree.h"
#include "PlayerMagnet/PlayerXDegree.h"

ANewWorldDiscoveryCharacter::ANewWorldDiscoveryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->AttachTo(GetCapsuleComponent());

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	LastCheckpoint = nullptr;
	MaxBalls = 1;
	MaxBoxes = 1;
	MaxShields = 1;
	MaxHoldingObjects = 1;

	currentEnergyIndex = 0;

	MagnetAbility = nullptr;

	bGodmode = false;
	Radius = 212.0f;
	timeToCircle = 1.0f;
	decreasingPower = 0.01f * 0.0175f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANewWorldDiscoveryCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveRight", this, &ANewWorldDiscoveryCharacter::MoveRight);
	InputComponent->BindAxis("RotateAround", this, &ANewWorldDiscoveryCharacter::RotateAround);

	InputComponent->BindAction("CreateBox", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CreateMagneticBox);
	InputComponent->BindAction("CreatePyramide", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CreateMagneticShields);
	InputComponent->BindAction("CreateBall", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CreateMagneticBall);

	InputComponent->BindAction("Reset", IE_Pressed, this, &ANewWorldDiscoveryCharacter::Reset);

	InputComponent->BindAction("EnableAbilitys", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CheatEnableAllAbilitys);

	InputComponent->BindTouch(IE_Pressed, this, &ANewWorldDiscoveryCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ANewWorldDiscoveryCharacter::TouchStopped);
}

void ANewWorldDiscoveryCharacter::CheatEnableAllAbilitys()
{
	AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)this->PlayerState;
	if (playerState != nullptr)
	{
		playerState->AddAbility(7);
		playerState->AddRoboPart(31);
	}
}

void ANewWorldDiscoveryCharacter::Reset()
{
	if (LastCheckpoint != nullptr)
	{ 
		for (int i = 0; i < CreatedBoxes.Num(); i++)
		{
			AMagneticBox *box = Cast<AMagneticBox>(CreatedBoxes[i]);
			if (box)
				box->TriggerDestroy(true);
		}
		CreatedBoxes.Empty();
		for (int i = 0; i < CreatedShields.Num(); i++)
		{
			AMagneticShield* provider = Cast<AMagneticShield>(CreatedShields[i]);
			if (provider)
				provider->TriggerDestroy(true);
		}
		CreatedShields.Empty();
		for (int i = 0; i < CreatedBalls.Num(); i++)
		{
			AMagneticEnergyTransfer *transfer = Cast<AMagneticEnergyTransfer>(CreatedBalls[i]);
			if (transfer)
				transfer->TriggerDestroy(true);
		}
		CreatedBalls.Empty();

		for (int i = 0; i < HoldingObjects.Num(); i++)
		{
			ABaseMagnetic* baseMagnetic = HoldingObjects[i];
			if (baseMagnetic)
			{
				if (!Cast<AMagneticShield>(baseMagnetic))
					baseMagnetic->TriggerDestroy(true);
			}
		}
		HoldingObjects.Empty();
		LastCheckpoint->Reset();
		OnReset();

		FVector ResetLocation = LastCheckpoint->GetActorLocation();
		ResetLocation.Z += 150.0f;

		SetActorLocation(ResetLocation);
	}
}

bool ANewWorldDiscoveryCharacter::RemoveEnergy()
{
	return true;

	if (currentEnergyIndex == EnergyCosts.Num())
	{
		currentEnergyIndex = 0;
	}
	
	AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)this->PlayerState;
	if (playerState != nullptr)
	{
		int32 currentEnergy = playerState->GetCurrentEnergy();
		int32 energyCosts = EnergyCosts[currentEnergyIndex];
		currentEnergyIndex++;

		if (currentEnergy >= energyCosts)
		{
			playerState->RemoveEnergy(energyCosts);
			return true;
		}
		else
			return false;
	}
	return false;
}

/*
 ToDo: More Generic ... More Abstract ... pretty much the same
*/
void ANewWorldDiscoveryCharacter::CreateMagneticBox()
{
	AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)this->PlayerState;
	if (playerState != nullptr)
	{
		if (!playerState->HasBoxAbility())
			return;
	}

	FVector SpawnLocation = GetSpawnLocation();
	if (!IsSpawnPossible(GetActorLocation(), SpawnLocation))
		return;

	//We have no energy .. so we cant create more objects
	if (!RemoveEnergy())
		return;

	if (CreatedBoxes.Num() >= MaxBoxes)
	{
		AMagneticBox *box = Cast<AMagneticBox>(CreatedBoxes[0]);
		if (box)
			box->TriggerDestroy(true);

		CreatedBoxes.RemoveAt(0);
	}

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FRotator Rotation = FRotator(0.0f,0.0f,0.0f);
	ABaseMagnetic *box = GetWorld()->SpawnActor<ABaseMagnetic>(MagneticBox,SpawnLocation, Rotation, SpawnParameters);
	if (box)
	{ 
		box->bIgnoreMagnetic = false;
		box->MagneticMesh->SetSimulatePhysics(false);
		box->MagneticMesh->SetEnableGravity(true);
		box->MagneticMesh->bGenerateOverlapEvents = false;
		box->MagneticMesh->bMultiBodyOverlap = true;
		box->MagneticMesh->SetCollisionProfileName("MagneticBox");
		box->bForceShit = false;
		CreatedBoxes.Add(box);
		OnCreate();
		box->OnCreate();
		
		if (obstacle != nullptr)
		{
			obstacle->DestroyMagneticObject();
			obstacle = nullptr;
		}
	}


}
void ANewWorldDiscoveryCharacter::CreateMagneticBall()
{
	AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)this->PlayerState;
	if (playerState != nullptr)
	{
		if (!playerState->HasBallAbility())
			return;
	}

	FVector SpawnLocation = GetSpawnLocation();
	if (!IsSpawnPossible(GetActorLocation(), SpawnLocation))
		return;

	//We have no energy .. so we cant create more objects
	if (!RemoveEnergy())
		return;

	if (CreatedBalls.Num() >= MaxBalls)
	{
		AMagneticEnergyTransfer *transfer = Cast<AMagneticEnergyTransfer>(CreatedBalls[0]);
		if (transfer)
			transfer->TriggerDestroy(true);

		CreatedBalls.RemoveAt(0);
	}

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	ABaseMagnetic *ball = GetWorld()->SpawnActor<ABaseMagnetic>(MagneticBall, SpawnLocation, Rotation, SpawnParameters);
	if (ball)
	{
		ball->bIgnoreMagnetic = false;
		ball->MagneticMesh->SetSimulatePhysics(false);
		ball->MagneticMesh->SetEnableGravity(true);
		ball->MagneticMesh->bGenerateOverlapEvents = false;
		ball->MagneticMesh->bMultiBodyOverlap = true;
		ball->bForceShit = false;
		ball->MagneticMesh->SetCollisionProfileName("MagneticBox");
		CreatedBalls.Add(ball);
		OnCreate();
		ball->OnCreate();
	}
		
}
void ANewWorldDiscoveryCharacter::CreateMagneticShields()
{
	AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)this->PlayerState;
	if (playerState != nullptr)
	{
		if (!playerState->HasShieldAbility())
			return;
	}

	FVector SpawnLocation = GetSpawnLocation();
	if (!IsSpawnPossible(GetActorLocation(), SpawnLocation))
		return;

	//We have no energy .. so we cant create more objects
	if (!RemoveEnergy())
		return;

	if (CreatedShields.Num() >= MaxShields)
	{
		AMagneticShield *provider = Cast<AMagneticShield>(CreatedShields[0]);
		if (provider)
			provider->TriggerDestroy(true);
		CreatedShields.RemoveAt(0);
	}

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	ABaseMagnetic *pyramide = GetWorld()->SpawnActor<ABaseMagnetic>(MagneticShields, SpawnLocation, Rotation, SpawnParameters);
	if (pyramide)
	{
		pyramide->bIgnoreMagnetic = false;
		pyramide->MagneticMesh->SetSimulatePhysics(false);
		pyramide->MagneticMesh->SetEnableGravity(true);
		pyramide->MagneticMesh->bGenerateOverlapEvents = false;
		pyramide->MagneticMesh->bMultiBodyOverlap = true;
		pyramide->MagneticMesh->SetCollisionProfileName("MagneticBox");
		pyramide->bForceShit = false;
		CreatedShields.Add(pyramide);
		OnCreate();
		pyramide->OnCreate();
	}
}

FVector ANewWorldDiscoveryCharacter::GetSpawnLocation()
{
	float xDir = 1.0f;
	FVector forward = GetActorForwardVector();
	xDir = forward.Y <= 0 ? -1.0f : 1.0f;
	FVector direction = FVector(0.0f, xDir, 1.0f);
	FVector SpawnLocation = SpawnPoint->RelativeLocation;
	SpawnLocation.Y = SpawnLocation.X;
	SpawnLocation = (SpawnLocation * direction) + GetActorLocation();

	return SpawnLocation;
}

bool ANewWorldDiscoveryCharacter::IsSpawnPossible(FVector startLocation,FVector endLocation)
{
	FCollisionQueryParams CollisionParameters(
		FName(TEXT("TraceGround")), // tag name (for debugging)
		false, // trace against simple collision primitives only
		this // ignore this actor during the trace
		);
	FHitResult Hit;
	FHitResult Hit2;

	bool blockingHit = GetWorld()->LineTraceSingle(Hit, startLocation, endLocation, ECollisionChannel::ECC_WorldStatic, CollisionParameters);
	bool blockingHit2 = GetWorld()->LineTraceSingle(Hit2, startLocation, endLocation, ECollisionChannel::ECC_WorldDynamic, CollisionParameters);

	if (blockingHit || blockingHit2)
	{
		//No Creation
		return false;
	}
	return true;
}

void ANewWorldDiscoveryCharacter::RotateAround(float Value)
{
	//if (Value > 0.0f)	//Always same speed
	//	Value = 1.0f;
	//else if (Value < 0.0f)
	//	Value = -1.0f;

	AWorldDiscoveryPlayerController *controller = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
	float MaxSensitivity = controller->InputYawScale;

	if (MaxSensitivity <= 0.0f)
		MaxSensitivity = 1.0f;

	Value = FMath::Clamp(Value,-MaxSensitivity, MaxSensitivity);

	OnRotateAround(Value);

	RotationCurrent = Value;

	for (int i = 0; i < HoldingObjects.Num(); i++)
	{
		ABaseMagnetic* BaseMagnetic = Cast<ABaseMagnetic>(HoldingObjects[i]);
		if (BaseMagnetic)
		{
			BaseMagnetic->SetRotationRate(Value);
		}
	}
}

void ANewWorldDiscoveryCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ANewWorldDiscoveryCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ANewWorldDiscoveryCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ANewWorldDiscoveryCharacter::EnableMagnetic()
{
	if (MagnetAbility)
	{
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.bNoFail = true;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		float direction = 1.0f;
		FVector forward = this->GetActorForwardVector();
		direction = forward.Y <= 0 ? -1.0f : 1.0f;

		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
		Rotation.Roll = 90.0f + 70.0f * direction;
		FVector ActorPos = GetActorLocation();
		ActorPos.Z += 20.0f;

		playerDegree = GetWorld()->SpawnActor<APlayerDegree>(MagnetAbility, ActorPos, Rotation, SpawnParameters);
		if (playerDegree)
		{
			FVector DegreeLocation = playerDegree->magneticTrigger->RelativeLocation;
			Radius = playerDegree->magneticTrigger->GetUnscaledSphereRadius();
			
			playerDegree->magneticTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			playerDegree->magneticTrigger->bGenerateOverlapEvents = true;
			playerDegree->magneticWave->Activate();
			if (playerDegree->bParticleRotation)
			{
				APlayerXDegree *degree = Cast<APlayerXDegree>(playerDegree);
				if (degree)
				{
					degree->magneticWaveSingle->Activate();
				}
			}
				
			TargetLocation = FVector::ZeroVector;
			playerDegree->parentCharacter = this;

			bMagneticEffect = true;
		}
	}
}

void ANewWorldDiscoveryCharacter::DisableMagnetic()
{
	if (playerDegree)
	{
		playerDegree->magneticTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		playerDegree->magneticTrigger->bGenerateOverlapEvents = false;
		playerDegree->magneticWave->Deactivate();
		playerDegree->K2_DestroyActor();
		playerDegree = nullptr;

		bMagneticEffect = false;
	}
}

void ANewWorldDiscoveryCharacter::BeginPlay()
{
	Super::BeginPlay();

	OriginPosition = GetActorLocation();
}

void ANewWorldDiscoveryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActorPos = GetActorLocation();
	if (bMagneticEffect && MagnetAbility != nullptr)
	{
		if (playerDegree)
		{			
			if (playerDegree->bParticleRotation)
			{
				playerDegree->SetActorLocation(ActorPos);

				if (TargetLocation == FVector::ZeroVector)
					TargetLocation = playerDegree->magneticTrigger->RelativeLocation;

				if (RotationCurrent != 0.0f)
				{
					FVector newLocation = HelperClass::RotateAround(FVector::ZeroVector, playerDegree->magneticTrigger->RelativeLocation, 0.0f, RotationCurrent * decreasingPower, 1.0f, 1.0f);
					FVector ForceDirection = (FVector::ZeroVector - newLocation).GetSafeNormal();
					TargetLocation = -ForceDirection * Radius;
				}
				playerDegree->magneticTrigger->SetRelativeLocation(TargetLocation);

			}
			else
			{
				FVector newLocation = GetActorLocation();
				newLocation.Z += 20.0f;
				playerDegree->SetActorLocation(newLocation);

				FRotator Rot = playerDegree->GetActorRotation();
				Rot.Roll -= DeltaTime * RotationCurrent * 180.0f;
				playerDegree->SetActorRotation(Rot);

			}

			ABaseMagnetic* baseMagnetic = GetActiveObject();
			if (baseMagnetic)
			{
				baseMagnetic->UpdateTargetLocation(playerDegree->magneticTrigger->GetComponentLocation());
			}
		}
	}

	//Force to OriginX to prevent Glitches and Slides, if we go to far off we move back to X
	UCharacterMovementComponent *movementComp = GetCharacterMovement();
	FVector velocity = movementComp->Velocity;
	if (ActorPos.X != OriginPosition.X)
	{
		float absDist = FMath::Abs(ActorPos.X - OriginPosition.X);
		if (absDist <= 0.1f)
			velocity.X = 0.0f;
		else
		{
			velocity.X = OriginPosition.X - ActorPos.X;
		}
	}
	else
	{
		velocity.X = 0.0f;
	}
	movementComp->Velocity = velocity;

}

ABaseMagnetic* ANewWorldDiscoveryCharacter::GetActiveObject()
{
	if (HoldingObjects.Num() > 0)
	{
		return HoldingObjects[0];
	}

	return nullptr;
}

void ANewWorldDiscoveryCharacter::SetLastCheckpoint(AActor* Checkpoint)
{
	this->LastCheckpoint = Checkpoint;
}

void ANewWorldDiscoveryCharacter::AddPulledObject(ABaseMagnetic* baseMagnetic)
{
	if (baseMagnetic != nullptr)
	{
		HoldingObjects.Add(baseMagnetic);	
	}
}
void ANewWorldDiscoveryCharacter::RemovePulledObject(ABaseMagnetic* baseMagnetic)
{
	if (baseMagnetic != nullptr)
	{
		if (HoldingObjects.Num() > 0)
			HoldingObjects.Remove(baseMagnetic);
	}
}

void ANewWorldDiscoveryCharacter::EmptyHoldingObjects()
{
	HoldingObjects.Empty();
}

APlayerDegree* ANewWorldDiscoveryCharacter::GetPlayerDegree()
{
	return this->playerDegree;
}

float ANewWorldDiscoveryCharacter::GetRadius()
{
	return this->Radius;
}

void ANewWorldDiscoveryCharacter::DoDamage()
{
	if (bGodmode)
		return;

	bIsReseting = true;
	OnDamaged();
}

void ANewWorldDiscoveryCharacter::Godmode()
{
	bGodmode = !bGodmode;
}

void ANewWorldDiscoveryCharacter::SetCurrentObjectHolder(ABaseObstacle* obstacle)
{
	this->obstacle = obstacle;
}
