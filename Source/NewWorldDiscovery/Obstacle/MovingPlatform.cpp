// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MovingPlatform.h"

#include "MagneticBox/MagneticBox.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(true);

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->bGenerateOverlapEvents = false;
	PlatformMesh->AttachTo(BaseComponent);

	PlatformEnergySocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformEnergySocketMesh"));
	PlatformEnergySocketMesh->bGenerateOverlapEvents = false;
	PlatformEnergySocketMesh->AttachTo(PlatformMesh);

	BoxCollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionTrigger"));
	BoxCollisionTrigger->bGenerateOverlapEvents = true;
	BoxCollisionTrigger->SetCollisionProfileName("PlatformTrigger");
	BoxCollisionTrigger->AttachTo(PlatformEnergySocketMesh);

	SpherePullingTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SpherePullingTrigger"));
	SpherePullingTrigger->bGenerateOverlapEvents = true;
	SpherePullingTrigger->SetCollisionProfileName("PlayerTrigger");
	SpherePullingTrigger->AttachTo(BoxCollisionTrigger);

	PlatformLineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformLineMesh"));
	PlatformLineMesh->bGenerateOverlapEvents = false;
	PlatformLineMesh->AttachTo(BaseComponent);

	Acceleration = 2.0f;
	MaxPlatformVelocity = 10.0f;
	CurrentVelocity = 0.0f;
	bFixedSpeed = true;

	Type = eTypeDirection::HORIZONTAL;
	InitialDirection = eInitialDirection::END;

	MoveDirection = 1.0f;

	actor = nullptr;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if ((Type == eTypeDirection::HORIZONTAL && InitialDirection == eInitialDirection::END) || (Type == eTypeDirection::VERTICAL && InitialDirection == eInitialDirection::START))
		MoveDirection = -1.0f;
	else
		MoveDirection = 1.0f;

	RelOriginPosition = PlatformMesh->RelativeLocation;
	OriginDirection = InitialDirection;
	bOriginActive = bActive;
	
}

void AMovingPlatform::ResetPlatform()
{
	CurrentVelocity = 0.0f;
	PlatformMesh->SetRelativeLocation(RelOriginPosition);
	InitialDirection = OriginDirection;
	bActive = bOriginActive;
}

//ToDo: Write generell Acceleration/Movement-Helper Class
void AMovingPlatform::Accelerate(float DeltaTime)
{
	CurrentVelocity += DeltaTime * Acceleration * MaxPlatformVelocity;
	CurrentVelocity = FMath::Clamp(CurrentVelocity, 0.0f, MaxPlatformVelocity);
}

// Called every frame
void AMovingPlatform::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bActive)
	{
		if (bFixedSpeed)
		{
			Accelerate(DeltaTime);

			FTransform LineTransform = PlatformLineMesh->GetRelativeTransform();
			FVector LineLocationVector = LineTransform.GetLocation();

			FVector min;
			FVector max;
			PlatformLineMesh->GetLocalBounds(min,max);

			FTransform PlatformTransform = PlatformMesh->GetRelativeTransform();
			FVector PlatformLocationVector = PlatformTransform.GetLocation();

			FVector End = LineLocationVector;
			FVector Start = FVector::ZeroVector;
			

			if (Type == eTypeDirection::HORIZONTAL)
			{
				Start.Y = End.Y + ((max.Z + min.Z) * LineTransform.GetScale3D().Z);
				PlatformLocationVector.Y += MoveDirection * CurrentVelocity * DeltaTime;

				DirectionSwitch(PlatformLocationVector.Y, Start.Y, End.Y);

			}
			else if (Type == eTypeDirection::VERTICAL)
			{
				Start.Z = End.Z - ((max.Z + min.Z) * LineTransform.GetScale3D().Z);
				PlatformLocationVector.Z += MoveDirection * CurrentVelocity * DeltaTime;

				DirectionSwitch(PlatformLocationVector.Z, End.Z, Start.Z);

			}

			PlatformMesh->SetRelativeLocation(PlatformLocationVector);
		
			if (actor != nullptr)
			{
				if (actor->IsActorBeingDestroyed())
				{
					actor = nullptr;
					return;
				}
				FVector RelLoc = PlatformEnergySocketMesh->RelativeLocation;
				FVector ActorLocation = GetActorLocation() + PlatformLocationVector;

				ActorLocation.Y -= RelLoc.X;
				ActorLocation.Z += RelLoc.Z;
				actor->SetActorLocation(ActorLocation);
				UE_LOG(LogTemp, Warning, TEXT("AcLoc %f %f %f"), ActorLocation.X, ActorLocation.Y, ActorLocation.Z);
			}			
		}
		else
		{
			//Todo: Energy Based... more energy more Speed or acceleration?
		}
	}
}

void AMovingPlatform::DirectionSwitch(float current,float start, float end)
{
	float oldDirection = MoveDirection;

	if (current >= start)
		MoveDirection = -1.0f;
	else if (current <= end)
		MoveDirection = 1.0f;

	if (oldDirection != MoveDirection)
		CurrentVelocity = 0.0f;
}

void AMovingPlatform::TriggerPlatform(bool bActiveState)
{
	OnTriggerPlatform(bActiveState);
	this->bActive = bActiveState;
}

void AMovingPlatform::SetStoringEnergy(uint32 energy)
{
	this->StoringEnergy = energy;
}

void AMovingPlatform::OverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMagneticBox *energyBox = Cast<AMagneticBox>(OtherActor);
	if (energyBox)
	{
		energyBox->TriggerMagneticStop();
		energyBox->MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
		energyBox->SetNewMassScale(20.0f);
		energyBox->bIgnoreMagnetic = true;
		

		TriggerPlatform(true);
	}
}

void AMovingPlatform::OverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMagneticBox *energyBox = Cast<AMagneticBox>(OtherActor);
	if (energyBox)
	{
		energyBox->MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Overlap);
		energyBox->bIgnoreMagnetic = false;
		energyBox->MagneticMesh->SetSimulatePhysics(true);
		energyBox->SetNewMassScale(1.0f);
		TriggerPlatform(false);
		actor = nullptr;
	}
}

float AMovingPlatform::GetRadius()
{
	return SpherePullingTrigger->GetUnscaledSphereRadius();
}
