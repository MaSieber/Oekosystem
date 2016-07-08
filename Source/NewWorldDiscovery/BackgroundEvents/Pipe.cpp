// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "Pipe.h"


// Sets default values
APipe::APipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	PipeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipeMesh"));
	PipeMesh->bGenerateOverlapEvents = false;
	PipeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PipeMesh->AttachTo(BaseComponent);

}

// Called when the game starts or when spawned
void APipe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APipe::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

