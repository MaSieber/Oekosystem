// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "BaseMagnetic.h"

#include "ObjectMagnet/ObjectMagnet.h"
#include "PlayerMagnet/PlayerDegree.h"

#include "../Obstacle/MovingPlatform.h"
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
	magneticMovement->bInitialVelocityInLocalSpace = true;
	magneticMovement->InitialSpeed = 5.0f;
	magneticMovement->bRotationFollowsVelocity = false;
	magneticMovement->Bounciness = 0.0f;
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
	RotationFollowVelocity = 860.0f;
	RotationPercentDistanceVelocity = 60.0f;

	bDestroying = false;

	ForceDirection = FVector::ZeroVector;

	bIgnoreMagnetic = false;

	magneticMovement->SetUpdatedComponent(NULL);

	bUpdating = false;

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

void ABaseMagnetic::UpdateTargetLocation(FVector Location)
{
	if (!bUpdating)
		return;
	TargetLocation = Location;
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
		
	FVector moveDirection = FVector::ZeroVector;
	FVector ActorPos = GetActorLocation();
	float percent = 1.0f;
	
	switch (PullingType)
	{
		case ePulling::NONE:
			break;
		case ePulling::PULLING:
		{			
			float Dist = FVector::Dist(TargetLocation, ActorPos);
			UE_LOG(LogTemp, Warning, TEXT("dist:%f,Radius: %f"), Dist, Radius);
			if (Dist > Radius)
			{
				TriggerMagneticStop();
				return;
			}
			else if (Dist <= Radius && Dist >= 5.0f)
			{
				moveDirection = (TargetLocation - ActorPos).GetSafeNormal();
				percent = Dist / RotationPercentDistanceVelocity;
			}			

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
		default:
			throw "Error: EnumVal not defined";
	} //end switch

	//aftermath
	if (PullingType != ePulling::NONE)
		MagneticMesh->SetPhysicsLinearVelocity(moveDirection * CurrentVelocity * percent);
	if (PullingType == ePulling::PULLING)
	{ 
		//Rotate
		FRotator rot = GetActorRotation();
		rot.Roll += RotationVelocity * FMath::Sin(DeltaTime);
		SetActorRotation(rot);
	}


}

void ABaseMagnetic::triggerMagnetic(FVector Location, bool bUpdating)
{
	if (bIgnoreMagnetic) return;

	TargetLocation = Location;

	this->bUpdating = bUpdating;
		
	MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	MagneticMesh->SetEnableGravity(false);
	MagneticMesh->SetSimulatePhysics(true);
	MagneticMesh->bGenerateOverlapEvents = true;
	MagneticMesh->bMultiBodyOverlap = true;
	PullingType = ePulling::PULLING;

	UE_LOG(LogTemp,Warning,TEXT("triggerMagnetic"));
	
}

void ABaseMagnetic::TriggerMagneticStop()
{
	PullingType = ePulling::NONE;
	MagneticMesh->SetCollisionProfileName("MagneticBox");
	MagneticMesh->SetEnableGravity(true);
	MagneticMesh->SetSimulatePhysics(true);
	MagneticMesh->bGenerateOverlapEvents = true;
	MagneticMesh->bMultiBodyOverlap = true;
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(parentCharacter);
	if (playerChar)
	{
		playerChar->EmptyHoldingObjects();
	}

	parentCharacter = nullptr;
	//Clearing of Pulled Objects with Blueprint

}

void ABaseMagnetic::TriggerMagneticPush()
{
	if (bIgnoreMagnetic) return;
	if (PullingType == ePulling::PULLING)
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
			this->K2_DestroyActor();
		}
		
	}
	else
	{
		//trigger some blueprint event
		bDestroying = true;
	}
}

void ABaseMagnetic::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnOverlap(OtherActor,true);
}

void ABaseMagnetic::OnOverlap(class AActor* actor,bool bState)
{
	if (bState)
	{
		if (bIgnoreMagnetic)
			return;

		if (PullingType == ePulling::NONE)
		{
			AObjectMagnet *magnet = Cast<AObjectMagnet>(actor);
			if (magnet)
			{
				UE_LOG(LogTemp,Warning,TEXT("ObjectMagnet"));
				FVector TargetLocation = magnet->GetActorLocation() + magnet->magneticTrigger->RelativeLocation;
				UE_LOG(LogTemp, Warning, TEXT("ObjectMagnet Actor z:%f,target z: %f"), GetActorLocation().Z,TargetLocation.Z);
				if (GetActorLocation().Z > TargetLocation.Z) //critical test
				{
					ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(parentCharacter);
					if (playerChar)
					{
						playerChar->RemovePulledObject(this);
					}

					parentCharacter = nullptr;
					Radius = magnet->Radius;
					CurrentVelocity = magnet->PullingVelocity;
					triggerMagnetic(TargetLocation, false);
				}
				return;
			}
			
			APlayerDegree *playerDegree = Cast<APlayerDegree>(actor);
			if (playerDegree)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerMagnet"));
				parentCharacter = playerDegree->parentCharacter;
				ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(parentCharacter);
				if (playerChar)
				{
					Radius = playerChar->GetRadius();
					playerChar->AddPulledObject(this);
					CurrentVelocity = RotationFollowVelocity;
				}
				triggerMagnetic(playerDegree->magneticTrigger->GetComponentLocation(), true);
			}
		}

	}
}

void ABaseMagnetic::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnOverlap(OtherActor, false);
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

bool ABaseMagnetic::IsDestroyed()
{
	return bIsDestroyed || bDestroying;
}
