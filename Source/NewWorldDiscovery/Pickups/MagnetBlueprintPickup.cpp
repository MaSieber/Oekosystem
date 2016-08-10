// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MagnetBlueprintPickup.h"

#include "../WorldDiscoveryPlayerState.h"
#include "../NewWorldDiscoveryCharacter.h"


// Sets default values
AMagnetBlueprintPickup::AMagnetBlueprintPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	PickupEventCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EventCollider"));
	PickupEventCollider->bGenerateOverlapEvents = true;
	PickupEventCollider->SetCollisionProfileName("MapEvent");
	PickupEventCollider->OnComponentBeginOverlap.AddDynamic(this, &AMagnetBlueprintPickup::OnOverlapBegin);
	PickupEventCollider->AttachTo(BaseComponent);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->bGenerateOverlapEvents = false;
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMesh->AttachTo(BaseComponent);

}

// Called when the game starts or when spawned
void AMagnetBlueprintPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagnetBlueprintPickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMagnetBlueprintPickup::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickupEventCollider->bGenerateOverlapEvents = false;
	PickupEventCollider->SetCollisionProfileName("NoCollision");
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)playerChar->PlayerState;
		if (playerState != nullptr)
		{
			playerState->AddAbility((int32)AbilityType);
			OnDestroying();
		}
	}
}

