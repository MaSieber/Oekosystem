// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MovingPlatformReset.h"

#include "../NewWorldDiscoveryCharacter.h"

// Sets default values
AMovingPlatformReset::AMovingPlatformReset()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	ResetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResetMesh"));
	ResetMesh->bGenerateOverlapEvents = false;
	ResetMesh->AttachTo(BaseComponent);

	BoxCollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionTrigger"));
	BoxCollisionTrigger->bGenerateOverlapEvents = true;
	BoxCollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatformReset::OnOverlapBegin);
	BoxCollisionTrigger->AttachTo(ResetMesh);

	ResetSocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResetSocketMesh"));
	ResetSocketMesh->bGenerateOverlapEvents = false;
	ResetSocketMesh->AttachTo(BaseComponent);

}

// Called when the game starts or when spawned
void AMovingPlatformReset::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingPlatformReset::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMovingPlatformReset::Reset()
{
	
	for (AMovingPlatform* platform : Platforms)
	{
		platform->ResetPlatform(false);
	}
}

void AMovingPlatformReset::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANewWorldDiscoveryCharacter *playerCharacter = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerCharacter)
	{
		Reset();
	}
}