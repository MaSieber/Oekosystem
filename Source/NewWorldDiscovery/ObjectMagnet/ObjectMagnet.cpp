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

	magneticTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	magneticTrigger->bGenerateOverlapEvents = false;
	magneticTrigger->SetSimulatePhysics(false);
	magneticTrigger->SetCollisionProfileName("ObjectMagnetTrigger");
	magneticTrigger->AttachTo(SceneComponent);

	magneticWave = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WaveParticle"));
	magneticWave->AttachTo(magneticTrigger);

	PullingVelocity = 500.0f;

}

// Called when the game starts or when spawned
void AObjectMagnet::BeginPlay()
{
	Super::BeginPlay();

	Radius = magneticTrigger->GetUnscaledSphereRadius() + 55.0f;
	
}

// Called every frame
void AObjectMagnet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

