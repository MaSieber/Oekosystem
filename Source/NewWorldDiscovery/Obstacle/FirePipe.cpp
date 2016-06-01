// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "FirePipe.h"


// Sets default values
AFirePipe::AFirePipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoilMesh"));
	FireMesh->bGenerateOverlapEvents = false;
	FireMesh->AttachTo(BaseComponent);

	CollisionTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionTrigger"));
	CollisionTrigger->bGenerateOverlapEvents = true;
	CollisionTrigger->SetCollisionProfileName("PlatformTrigger");
	CollisionTrigger->AttachTo(FireMesh);

	fireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	fireParticle->bGenerateOverlapEvents = false;
	fireParticle->bAutoActivate = true;
	fireParticle->AttachTo(FireMesh);

}

// Called when the game starts or when spawned
void AFirePipe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirePipe::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

