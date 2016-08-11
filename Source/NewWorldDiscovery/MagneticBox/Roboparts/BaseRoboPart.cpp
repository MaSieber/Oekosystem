// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "BaseRoboPart.h"


// Sets default values
ABaseRoboPart::ABaseRoboPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FixPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FixPointMesh"));
	FixPointMesh->bGenerateOverlapEvents = false;
	FixPointMesh->bMultiBodyOverlap = true;
	FixPointMesh->SetCollisionProfileName("NoCollision");
	FixPointMesh->SetSimulatePhysics(false);
	FixPointMesh->AttachTo(MagneticMesh);

	RoboMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoboMesh"));
	RoboMesh->bGenerateOverlapEvents = false;
	RoboMesh->bMultiBodyOverlap = true;
	RoboMesh->SetCollisionProfileName("NoCollision");
	RoboMesh->SetSimulatePhysics(false);
	RoboMesh->AttachTo(FixPointMesh);

	bIgnoreMagnetic = false;
	MagneticMesh->SetSimulatePhysics(false);
	MagneticMesh->SetEnableGravity(true);
	MagneticMesh->bGenerateOverlapEvents = true;
	MagneticMesh->bMultiBodyOverlap = true;
	MagneticMesh->SetCollisionProfileName("RoboPart");

	Type = 999;
}

// Called when the game starts or when spawned
void ABaseRoboPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseRoboPart::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

