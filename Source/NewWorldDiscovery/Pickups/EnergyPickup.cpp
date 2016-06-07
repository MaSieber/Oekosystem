// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "EnergyPickup.h"
#include "WorldDiscoveryPlayerState.h"
#include "NewWorldDiscoveryCharacter.h"


// Sets default values
AEnergyPickup::AEnergyPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	PickupEventCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EventCollider"));
	PickupEventCollider->bGenerateOverlapEvents = true;
	PickupEventCollider->SetCollisionProfileName("MapEvent");
	PickupEventCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnergyPickup::OnOverlapBegin);
	PickupEventCollider->AttachTo(BaseComponent);

}

// Called when the game starts or when spawned
void AEnergyPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnergyPickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AEnergyPickup::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickupEventCollider->bGenerateOverlapEvents = false;
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)playerChar->PlayerState;
		if (playerState != nullptr)
		{
			playerState->AddRessource(ProvidingRessource);
			UE_LOG(LogTemp,Warning,TEXT("%d"),ProvidingRessource);

			OnPickup();
		}
	}


}
