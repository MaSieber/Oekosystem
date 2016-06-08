// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "PickupRoboterPart.h"

#include "../NewWorldDiscoveryCharacter.h"
#include "../WorldDiscoveryPlayerState.h"

// Sets default values
APickupRoboterPart::APickupRoboterPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	PickupEventCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EventCollider"));
	PickupEventCollider->bGenerateOverlapEvents = true;
	PickupEventCollider->SetCollisionProfileName("MapEvent");
	PickupEventCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupRoboterPart::OnOverlapBegin);
	PickupEventCollider->AttachTo(BaseComponent);

	RoboPartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoboPartMesh"));
	RoboPartMesh->bGenerateOverlapEvents = false;
	RoboPartMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RoboPartMesh->AttachTo(BaseComponent);

}

// Called when the game starts or when spawned
void APickupRoboterPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupRoboterPart::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickupRoboterPart::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickupEventCollider->bGenerateOverlapEvents = false;
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		AWorldDiscoveryPlayerState * playerState = (AWorldDiscoveryPlayerState*)playerChar->PlayerState;
		if (playerState != nullptr)
		{
			playerState->AddRoboPart((int32)AWorldDiscoveryPlayerState::eRoboParts::HEAD);
			K2_DestroyActor();

		}
	}
}