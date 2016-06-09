// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "BaseMagnetic.h"

#include "NewWorldDiscoveryCharacter.h"
#include "WorldDiscoveryPlayerController.h"
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

	MovementComponent = CreateDefaultSubobject<UMovementComponent>(TEXT("MovementComponent"));
	AddComponent(FName("MovementComponent"), false, FTransform(), nullptr);

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

	if (MovementComponent)
		MovementComponent->Velocity = FVector::ZeroVector;

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
		return;

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
		if (dist >= (Radius + 50.0f))
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
			playerChar->AddPulledObject(this);
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

		TargetLocation += -ForceDirection * Radius;
		MagneticMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		//Set Location to Player Location
		FVector newLocation = TargetLocation;

		//FQuat rotQuat = GetActorRotation().Quaternion();
		//bool bOverlapTest = MovementComponent->OverlapTest(newLocation, rotQuat, ECollisionChannel::ECC_WorldStatic, MagneticMesh->GetCollisionShape(), this);
		//bool bOverlapTest2 = MovementComponent->OverlapTest(newLocation, rotQuat, ECollisionChannel::ECC_GameTraceChannel3, MagneticMesh->GetCollisionShape(), this);
		//if (!bOverlapTest)
		{
			RotationRate *= -1.0f;
			SetActorLocation(newLocation);
		}

		//Rotate
		FRotator rot = GetActorRotation();
		rot.Roll += xDir * RotationVelocity * FMath::Sin(DeltaTime);
		SetActorRotation(rot);

		if (RotationRate != 0.0f)
		{
			//Rotate Around
			float MouseY = (RotationCurrent * 360);		//percent base x percent of 360
			float s = RotationAmplitude * FMath::Sin(RotationFrequency * MouseY);
			float c = RotationAmplitude * FMath::Cos(RotationFrequency * MouseY);

			FVector box = GetActorLocation();
			FVector player = playerChar->GetActorLocation();

			float newY = player.Y + (c * (box.Y - player.Y) - s * (box.Z - player.Z));
			float newZ = player.Z + (s * (box.Y - player.Y) + c * (box.Z - player.Z));

			FVector newLocation = FVector(StaticXPos, newY, newZ);
			ForceDirection = (player - newLocation).GetSafeNormal();
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
	}
}

void ABaseMagnetic::triggerMagnetic(FVector direction, float force)
{
	if (bIgnoreMagnetic) return;
	if (PullingType == ePulling::NONE)
	{
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

		AWorldDiscoveryPlayerController* playerController = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
		if (playerController)
		{
			ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(playerController->GetCharacter());
			if (playerChar)
			{
				parentCharacter = playerChar;
			}
		}
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

		UE_LOG(LogTemp, Warning, TEXT("ABaseMagnetic - OnOverlap"));
		ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(actor);
		if (playerChar)
		{
			FVector playerPos = playerChar->GetActorLocation();
			playerPos.Z += 200.0f;

			FVector direction = playerPos - MagneticMesh->RelativeLocation;
			triggerMagnetic(direction, 1500);
		}
		else
			bStop = true;
	}
	else
	{
		ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(actor);
		if (playerChar)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABaseMagnetic - OnOverlap End"));
			//PullingType = ePulling::NONE;
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

