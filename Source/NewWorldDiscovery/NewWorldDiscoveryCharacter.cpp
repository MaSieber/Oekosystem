// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "NewWorldDiscovery.h"
#include "NewWorldDiscoveryCharacter.h"

#include "WorldDiscoveryPlayerState.h"

#include "DrawDebugHelpers.h"

#include "MagneticBox/BaseMagnetic.h"
#include "MagneticBox/MagneticBox.h"
#include "MagneticBox/MagneticEnergyProvider.h"
#include "MagneticBox/MagneticEnergyTransfer.h"
#include "MagneticBox/MagneticShield.h"

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

	magneticTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));	
	magneticTrigger->bGenerateOverlapEvents = false;
	magneticTrigger->SetSimulatePhysics(false);
	magneticTrigger->AttachTo(RootComponent);
	
	LastCheckpoint = nullptr;
	MaxBalls = 1;
	MaxBoxes = 1;
	MaxPyramides = 1;

	currentEnergyIndex = 0;

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
	InputComponent->BindAction("CreatePyramide", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CreateMagneticPyramide);
	InputComponent->BindAction("CreateBall", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CreateMagneticBall);

	InputComponent->BindAction("Reset", IE_Pressed, this, &ANewWorldDiscoveryCharacter::Reset);

	InputComponent->BindTouch(IE_Pressed, this, &ANewWorldDiscoveryCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ANewWorldDiscoveryCharacter::TouchStopped);
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
		for (int i = 0; i < CreatedPyramides.Num(); i++)
		{
			AMagneticEnergyProvider* provider = Cast<AMagneticEnergyProvider>(CreatedPyramides[i]);
			if (provider)
				provider->TriggerDestroy(true);
		}
		CreatedPyramides.Empty();
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
		box->MagneticMesh->SetSimulatePhysics(false);
		box->MagneticMesh->bGenerateOverlapEvents = false;
		CreatedBoxes.Add(box);
		box->OnCreate();
	}
}
void ANewWorldDiscoveryCharacter::CreateMagneticBall()
{
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
		ball->MagneticMesh->SetSimulatePhysics(false);
		ball->MagneticMesh->bGenerateOverlapEvents = false;
		CreatedBalls.Add(ball);
		ball->OnCreate();
	}
		
}
void ANewWorldDiscoveryCharacter::CreateMagneticPyramide()
{
	FVector SpawnLocation = GetSpawnLocation();
	if (!IsSpawnPossible(GetActorLocation(), SpawnLocation))
		return;

	//We have no energy .. so we cant create more objects
	if (!RemoveEnergy())
		return;

	if (CreatedPyramides.Num() >= MaxPyramides)
	{
		AMagneticEnergyProvider *provider = Cast<AMagneticEnergyProvider>(CreatedPyramides[0]);
		if (provider)
			provider->TriggerDestroy(true);
		CreatedPyramides.RemoveAt(0);
	}

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	ABaseMagnetic *pyramide = GetWorld()->SpawnActor<ABaseMagnetic>(MagneticPyramide, SpawnLocation, Rotation, SpawnParameters);
	if (pyramide)
	{
		pyramide->MagneticMesh->SetSimulatePhysics(false);
		pyramide->MagneticMesh->bGenerateOverlapEvents = false;
		CreatedPyramides.Add(pyramide);
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

void ANewWorldDiscoveryCharacter::Tick(float DeltaTime)
{

	UCharacterMovementComponent *movementComp = GetCharacterMovement();
	FVector velocity = movementComp->Velocity;
	velocity.X = 0.0f;
	movementComp->Velocity = velocity;

	//To Trigger the Collision Event, if we stay still
	float rad = magneticTrigger->GetUnscaledSphereRadius();
	magneticTrigger->SetSphereRadius(rad + FMath::Sin(DeltaTime)/10.0f);
}

ABaseMagnetic* ANewWorldDiscoveryCharacter::GetActiveObject()
{
	UE_LOG(LogTemp,Warning,TEXT("%d"), HoldingObjects.Num());

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
