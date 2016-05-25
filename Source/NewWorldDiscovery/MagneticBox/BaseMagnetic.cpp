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
	MagneticMesh->bGenerateOverlapEvents = true;
	MagneticMesh->bMultiBodyOverlap = true;
	MagneticMesh->SetCollisionProfileName("MagneticBox");
	MagneticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseMagnetic::OnOverlapBegin);
	MagneticMesh->OnComponentEndOverlap.AddDynamic(this, &ABaseMagnetic::OnOverlapEnd);
	MagneticMesh->SetSimulatePhysics(true);

	RootComponent = MagneticMesh;

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
}

void ABaseMagnetic::BeginPlay()
{
	Super::BeginPlay();

	StaticXPos = GetActorLocation().X;
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
		OnDestroying();
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
		if (dist > 10)
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
		ForceDirection = (TargetLocation - ActorPos).GetSafeNormal();
		TargetLocation += -ForceDirection * Radius;
		
		MagneticMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		FVector phyVel = MagneticMesh->GetPhysicsLinearVelocity();
		//UE_LOG(LogTemp, Warning, TEXT("phyVel - %f %f %f"), phyVel.X, phyVel.Y, phyVel.Z);

		//Set Location to Player Location
		FVector newLocation = TargetLocation;
		SetActorLocation(TargetLocation);
		
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
			//UE_LOG(LogTemp, Warning, TEXT("NewLocation - %f %f %f"), newLocation.X,newLocation.Y,newLocation.Z);
			SetActorLocation(newLocation);

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

		//PullingType = ePulling::NONE;

		break;
	}
	}
}

void ABaseMagnetic::triggerMagnetic(FVector direction, float force)
{
	if (PullingType == ePulling::NONE)
	{
		MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		ForceDirection = direction.GetSafeNormal();
		ForceAmount = force;
		CurrentForceSeconds = ForceSeconds;
		MagneticMesh->SetEnableGravity(false);
		MagneticMesh->SetSimulatePhysics(true);

		MagneticMesh->bGenerateOverlapEvents = false;
		MagneticMesh->bMultiBodyOverlap = false;

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

}

void ABaseMagnetic::TriggerMagneticPush()
{
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
			}
		}
	}
}

void ABaseMagnetic::TriggerDestroy(bool bInstant)
{
	if (bInstant)
		this->K2_DestroyActor();
	else
	{
		//trigger some blueprint event
		bDestroying = true;
	}
}

void ABaseMagnetic::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseMagnetic - OnOverlapBegin"));

	OnOverlap(OtherActor);
}

void ABaseMagnetic::OnOverlap(class AActor* actor)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseMagnetic - OnOverlap"));

	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(actor);
	if (playerChar)
	{
		FVector playerPos = playerChar->GetActorLocation();
		playerPos.Z += 200.0f;

		FVector direction = playerPos - MagneticMesh->RelativeLocation;
		triggerMagnetic(direction, 1500);
	}
}

void ABaseMagnetic::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseMagnetic - OnOverlap End"));
		//PullingType = ePulling::NONE;
	}
}

