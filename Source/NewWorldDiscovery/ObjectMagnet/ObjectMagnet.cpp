// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "ObjectMagnet.h"


// Sets default values
AObjectMagnet::AObjectMagnet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	magneticTrigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionTrigger"));
	magneticTrigger->bGenerateOverlapEvents = false;
	magneticTrigger->SetSimulatePhysics(false);
	magneticTrigger->SetCollisionProfileName("ObjectMagnet");
	magneticTrigger->AttachTo(SceneComponent);

	UnrealFickDich = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionTrigger2"));
	UnrealFickDich->bGenerateOverlapEvents = false;
	UnrealFickDich->SetSimulatePhysics(false);
	UnrealFickDich->SetCollisionProfileName("ObjectMagnet");
	UnrealFickDich->AttachTo(SceneComponent);

	magneticWave = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WaveParticle"));
	magneticWave->AttachTo(magneticTrigger);

	PullingVelocity = 500.0f;
	Radius = 200.0f;
}

// Called when the game starts or when spawned
void AObjectMagnet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectMagnet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

