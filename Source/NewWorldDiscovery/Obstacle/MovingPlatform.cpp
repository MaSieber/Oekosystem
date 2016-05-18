// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MovingPlatform.h"

#include "MagneticBox/MagneticEnergyProvider.h"

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

	BoxCollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionTrigger"));
	BoxCollisionTrigger->bGenerateOverlapEvents = true;
	BoxCollisionTrigger->SetCollisionProfileName("PlatformTrigger");


	//doenst work because, 2 blueprints ref to 1 c++ class ?
	//BoxCollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapBegin);
	//BoxCollisionTrigger->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapEnd);

	BoxCollisionTrigger->AttachTo(PlatformMesh);

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
	FMath::Clamp(CurrentVelocity, 0.0f, MaxPlatformVelocity);
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
	this->bActive = bActiveState;
}

void AMovingPlatform::SetStoringEnergy(uint32 energy)
{
	this->StoringEnergy = energy;
}

void AMovingPlatform::OverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMagneticEnergyProvider *energyProvider = Cast<AMagneticEnergyProvider>(OtherActor);
	if (energyProvider)
	{
		SetStoringEnergy(energyProvider->MaxEnergy);
		TriggerPlatform(true);
	}
}

void AMovingPlatform::OverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMagneticEnergyProvider *energyProvider = Cast<AMagneticEnergyProvider>(OtherActor);
	if (energyProvider)
	{
		SetStoringEnergy(0);
		TriggerPlatform(false);
	}
}

