// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "BatteryStation.h"
#include "WorldDiscoveryPlayerState.h"
#include "NewWorldDiscoveryCharacter.h"

#include "../MagneticBox/BaseMagnetic.h"
#include "../Obstacle/MovingPlatform.h"

// Sets default values
ABatteryStation::ABatteryStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCheckpoint = false;
	bInstantEnergy = false;
	bEnergyRefuel = false;

	EnergyRefuelTime = 5.0f;
	CurrentEnergyRefuelTime = 0.0f;
	
	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	BatteryEventCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EventCollider"));
	BatteryEventCollider->bGenerateOverlapEvents = true;
	BatteryEventCollider->SetCollisionProfileName("MapEvent");
	BatteryEventCollider->OnComponentBeginOverlap.AddDynamic(this, &ABatteryStation::OnOverlapBegin);
	BatteryEventCollider->AttachTo(BaseComponent);

	BatteryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryMesh"));
	BatteryMesh->AttachTo(BaseComponent);

}

// Called when the game starts or when spawned
void ABatteryStation::BeginPlay()
{
	Super::BeginPlay();

	CurrentEnergy = ProvidingEnergy;
}

// Called every frame
void ABatteryStation::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bEnergyRefuel)
	{
		if (CurrentEnergyRefuelTime <= 0.0f)
		{
			bEnergyRefuel = false;
			OnEnergyRise();
			CurrentEnergy = ProvidingEnergy;
		}
		else 
			CurrentEnergyRefuelTime -= DeltaTime;
	}
}

void ABatteryStation::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		if (bCheckpoint)
		{
			//Reset Player
			playerChar->SetLastCheckpoint(this);
			//Reset Platforms
		}

		AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)playerChar->PlayerState;
		if (playerState != nullptr)
		{
			if (bInstantEnergy)
			{
				playerState->AddEnergy(CurrentEnergy);
				CurrentEnergy = 0;
				CurrentEnergyRefuelTime = EnergyRefuelTime;
				bEnergyRefuel = true;
				OnEnergyFall();
			}
			else
				;//Some Animation,.... Energy slowly sucking out.
		}
	}

}

void ABatteryStation::Reset()
{
	if (bCheckpoint)
	{
		for (int i = 0; i < ObjectsToReset.Num(); i++)
		{
			AActor* actor = ObjectsToReset[i];

			if (AMovingPlatform* platform = Cast<AMovingPlatform>(actor))
			{
				platform->ResetPlatform();
			}
			else if (ABaseMagnetic* magnetic = Cast<ABaseMagnetic>(actor))
			{
				magnetic->Reset();
			}
		}
	}
}