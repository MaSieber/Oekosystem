// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "BaseMagnetic.h"

#include "PlayerMagnet/PlayerDegree.h"
#include "NewWorldDiscoveryCharacter.h"
#include "WorldDiscoveryPlayerController.h"
#include "../HelperClass.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABaseMagnetic::ABaseMagnetic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MagneticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagneticMesh"));
	MagneticMesh->bGenerateOverlapEvents = false;
	MagneticMesh->bMultiBodyOverlap = true;
	MagneticMesh->SetCollisionProfileName("MagneticBox");
	MagneticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseMagnetic::OnOverlapBegin);
	MagneticMesh->OnComponentEndOverlap.AddDynamic(this, &ABaseMagnetic::OnOverlapEnd);
	MagneticMesh->SetSimulatePhysics(true);

	RootComponent = MagneticMesh;

	magneticMovement = CreateDefaultSubobject<UProjectileMovementComponent>("MagneticMovmentComponent");
	magneticMovement->Velocity = FVector::ZeroVector;
	magneticMovement->ProjectileGravityScale = 0.0f;
	magneticMovement->bInitialVelocityInLocalSpace = false;
	magneticMovement->InitialSpeed = 5.0f;
	magneticMovement->bRotationFollowsVelocity = false;
	magneticMovement->Bounciness = 0.4f;
	magneticMovement->Friction = 0.0f;
	magneticMovement->BounceVelocityStopSimulatingThreshold = -1.0f;
	magneticMovement->SetIsReplicated(false);

	ForceSeconds = 1.0f;
	CurrentForceSeconds = 0.0f;

	PullingType = ePulling::NONE;

	CurrentVelocity = 0;
	PullVelocity = 100;
	PullAcceleration = 20;

	RotationVelocity = 100.0f;
	RotationAmplitude = 1.0f;
	RotationFrequency = 0.02f;
	RotationAroundVelocity = 0.010f;
	PushAmount = 5000.0f;
	Radius = 150.0f;

	bDestroying = false;

	ForceDirection = FVector::ZeroVector;

	bStop = false;
	bIgnoreMagnetic = false;

	magneticMovement->SetUpdatedComponent(NULL);

}

void ABaseMagnetic::BeginPlay()
{
	Super::BeginPlay();

	StaticXPos = GetActorLocation().X;

	OriginLocation = GetActorLocation();
	OriginRotation = GetActorRotation();
}

void ABaseMagnetic::SetRotationRate(float Value)
{
	RotationRate = Value;

	RotationCurrent = RotationRate * RotationAroundVelocity;
	FMath::Clamp(RotationCurrent,-1.0f,1.0f);
}

void ABaseMagnetic::Accelerate(float DeltaTime)
{
	CurrentVelocity += DeltaTime * PullAcceleration * PullVelocity;
	CurrentVelocity = FMath::Clamp(CurrentVelocity, 0.0f, PullVelocity);
}

// Called every frame
void ABaseMagnetic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDestroying && !bIsDestroyed)
	{
		bIsDestroyed = true;

		if (parentCharacter)
		{
			ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(parentCharacter);
			playerChar->RemovePulledObject(this);
		}

		OnDestroying();
		return;
	}

	if (bIgnoreMagnetic)
	{
		TriggerMagneticStop();
		return;
	}
		
	switch (PullingType)
	{
	case ePulling::NONE:
		break;
	case ePulling::PULLING:
	{
		ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		FVector ActorPos = GetActorLocation();
		float xDir = 1.0f;
		FVector forward = playerChar->GetActorForwardVector();
		xDir = forward.Y <= 0 ? -1.0f : 1.0f;

		TargetLocation = playerChar->GetActorLocation();
		ForceDirection = (TargetLocation - ActorPos).GetSafeNormal();
		TargetLocation += -ForceDirection * Radius;

		float dist = FVector::Dist(TargetLocation, ActorPos);
		if (dist >= (Radius + 10.0f))
		{
			TriggerMagneticStop();
		}
		else if (dist > 10)
		{
			ForceDirection = (TargetLocation - ActorPos).GetSafeNormal();
			Accelerate(DeltaTime);
			FVector newLocation = ActorPos + CurrentVelocity * ForceDirection * DeltaTime;
			DrawDebugLine(GetWorld(), ActorPos, TargetLocation, FColor(255, 255, 0, 1));
			SetActorLocation(newLocation);

			//FRotator rot = GetActorRotation();
			//rot.Roll += xDir * 80.0f * FMath::Sin(DeltaTime);
			//SetActorRotation(rot);

		}
		else
		{
			PullingType = ePulling::FOLLOWING;
		}
		break;
	}
	case ePulling::FOLLOWING:
	{
		AWorldDiscoveryPlayerController* playerController = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
		ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(playerController->GetCharacter());
		FVector ActorPos = GetActorLocation();

		float xDir = 1.0f;
		FVector forward = playerChar->GetActorForwardVector();
		xDir = forward.Y <= 0 ? -1.0f : 1.0f;

		TargetLocation = playerChar->GetActorLocation();
		if (ForceDirection == FVector::ZeroVector)
			ForceDirection = (TargetLocation - ActorPos).GetSafeNormal();

		if (RotationRate != 0.0f)
		{
			//Rotate Around
			FVector box = GetActorLocation();
			FVector player = playerChar->GetActorLocation();

			FVector newLocation = HelperClass::RotateAround(player, box, StaticXPos, RotationCurrent, RotationAmplitude, RotationFrequency);
			ForceDirection = (player - newLocation).GetSafeNormal();

		}
		TargetLocation += -ForceDirection * Radius;
		
		//Set Location to Player Location
		FVector newLocation = TargetLocation;
		FVector moveDirection = newLocation - GetActorLocation();

		APlayerDegree* degree = playerChar->GetPlayerDegree();
		if (degree)
		{
			FVector TriggerLocation = degree->GetActorLocation() + degree->magneticTrigger->RelativeLocation;

			float Dist = FVector::Dist(TriggerLocation, GetActorLocation());
			if (Dist >= 170.0f)
			{
				TriggerMagneticStop();
				playerChar->EmptyHoldingObjects();
			}
			else if (Dist <= 120.0f && Dist >= 30.0f)
			{
				//moveDirection = TriggerLocation - GetActorLocation();
			}
		}

		MagneticMesh->SetPhysicsLinearVelocity(moveDirection * 110.0f);

		//magneticMovement->Velocity = moveDirection * 120.0f;

		//Rotate
		FRotator rot = GetActorRotation();
		rot.Roll += xDir * RotationVelocity * FMath::Sin(DeltaTime);
		SetActorRotation(rot);
		break;

	}
	case ePulling::PUSHING:
	{
		AWorldDiscoveryPlayerController* playerController = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
		ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(playerController->GetCharacter());
		FVector ActorPos = GetActorLocation();
		FVector PlayerPos = playerChar->GetActorLocation();

		FVector PushDirection = -1.0f * (PlayerPos - ActorPos).GetSafeNormal();

		MagneticMesh->AddImpulseAtLocation(PushDirection * PushAmount, GetActorLocation());

		PullingType = ePulling::NONE;

		break;
	}
	}
}

void ABaseMagnetic::triggerMagnetic(FVector direction, float force)
{
	UE_LOG(LogTemp, Warning, TEXT("triggerMagnetic"));
	if (bIgnoreMagnetic) return;
	if (PullingType == ePulling::NONE)
	{

		UE_LOG(LogTemp,Warning,TEXT("triggerMagnetic - NONE"));

		AWorldDiscoveryPlayerController* playerController = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
		if (playerController)
		{
			ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(playerController->GetCharacter());
			if (playerChar)
			{
				if (playerChar->HoldingObjects.Num() >= playerChar->MaxHoldingObjects)
					return;


				UE_LOG(LogTemp, Warning, TEXT("triggerMagnetic - playerChar"));

				parentCharacter = playerChar;
				playerChar->AddPulledObject(this);
			}
		}

		MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		//SetNewMassScale(1.0f);

		ForceDirection = direction.GetSafeNormal();
		ForceAmount = force;
		CurrentForceSeconds = ForceSeconds;
		MagneticMesh->SetEnableGravity(false);
		MagneticMesh->SetSimulatePhysics(true);

		MagneticMesh->bGenerateOverlapEvents = true;
		MagneticMesh->bMultiBodyOverlap = true;

		PullingType = ePulling::PULLING;
		CurrentVelocity = 0;
	}
}

void ABaseMagnetic::TriggerMagneticStop()
{
	PullingType = ePulling::NONE;
	MagneticMesh->SetCollisionProfileName("MagneticBox");
	MagneticMesh->SetEnableGravity(true);
	MagneticMesh->SetSimulatePhysics(true);
	MagneticMesh->bGenerateOverlapEvents = true;
	MagneticMesh->bMultiBodyOverlap = true;

	parentCharacter = nullptr;
	//Clearing of Pulled Objects with Blueprint

}

void ABaseMagnetic::TriggerMagneticPush()
{
	if (bIgnoreMagnetic) return;
	if (PullingType == ePulling::FOLLOWING)
	{
		UE_LOG(LogTemp, Warning, TEXT("Push"));

		PullingType = ePulling::PUSHING;
		MagneticMesh->SetCollisionProfileName("MagneticBox");
		MagneticMesh->SetEnableGravity(true);
		MagneticMesh->SetSimulatePhysics(true);
		MagneticMesh->bGenerateOverlapEvents = true;
		MagneticMesh->bMultiBodyOverlap = true;

		AWorldDiscoveryPlayerController* playerController = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
		if (playerController)
		{
			ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(playerController->GetCharacter());
			if (playerChar)
			{
				playerChar->RemovePulledObject(this);
				parentCharacter = nullptr;
			}
		}
	}
}

void ABaseMagnetic::TriggerDestroy(bool bInstant)
{
	if (bInstant)
	{
		if (!bDestroying)
		{ 
			ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(parentCharacter);
			if (playerChar)
			{
				playerChar->RemovePulledObject(this);
			}
		}
		this->K2_DestroyActor();
	}
	else
	{
		//trigger some blueprint event
		bDestroying = true;
	}
}

void ABaseMagnetic::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseMagnetic - OnOverlapBegin"));

	OnOverlap(OtherActor,true);
}

void ABaseMagnetic::OnOverlap(class AActor* actor,bool bState)
{
	if (bState)
	{
		if (bIgnoreMagnetic)
			return;

		APlayerDegree *playerCollision = Cast<APlayerDegree>(actor);
		if (playerCollision)
		{
			FVector playerPos = playerCollision->GetActorLocation();
			playerPos.Z += 200.0f;

			FVector direction = playerPos - MagneticMesh->RelativeLocation;
			triggerMagnetic(direction, 1500);
		}
		else
			bStop = true;
	}
	else
	{
		APlayerDegree *playerCollision = Cast<APlayerDegree>(actor);
		if (playerCollision)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABaseMagnetic - OnOverlap End"));
			//PullingType = ePulling::NONE;
			//TriggerMagneticStop();
		}
	}
}

void ABaseMagnetic::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnOverlap(OtherActor, false);
	bStop = false;
}

bool ABaseMagnetic::IsInteractible()
{
	return (PullingType == ePulling::NONE);
}

void ABaseMagnetic::Reset()
{
	SetActorLocation(OriginLocation);
	SetActorRotation(OriginRotation);
	TriggerMagneticStop();
}

void ABaseMagnetic::SetNewMassScale(const float& NewScale)
{
	if (!MagneticMesh) return;

	FBodyInstance *BodyInst = MagneticMesh->GetBodyInstance();
	if (!BodyInst) return;

	BodyInst->MassScale = NewScale;
	BodyInst->UpdateMassProperties();
}

