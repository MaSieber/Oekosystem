// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "PlayerDegree.h"


// Sets default values
APlayerDegree::APlayerDegree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	magneticTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	magneticTrigger->bGenerateOverlapEvents = false;
	magneticTrigger->SetSimulatePhysics(false);
	magneticTrigger->SetCollisionProfileName("PlayerTrigger");
	magneticTrigger->AttachTo(SceneComponent);

	magneticWave = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WaveParticle"));
	magneticWave->AttachTo(magneticTrigger);

	parentCharacter = nullptr;

}

// Called when the game starts or when spawned
void APlayerDegree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerDegree::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
