// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "BaseRoboPart.h"


// Sets default values
ABaseRoboPart::ABaseRoboPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIgnoreMagnetic = false;
	MagneticMesh->SetSimulatePhysics(false);
	MagneticMesh->SetEnableGravity(true);
	MagneticMesh->bGenerateOverlapEvents = true;
	MagneticMesh->bMultiBodyOverlap = true;
	MagneticMesh->SetCollisionProfileName("RoboPart");

	ScalingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScalingMesh"));
	ScalingMesh->bGenerateOverlapEvents = false;
	ScalingMesh->bMultiBodyOverlap = false;
	ScalingMesh->SetCollisionProfileName("NoCollision");
	ScalingMesh->SetSimulatePhysics(false);
	ScalingMesh->AttachTo(MagneticMesh);

	FixPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FixPointMesh"));
	FixPointMesh->bGenerateOverlapEvents = false;
	FixPointMesh->bMultiBodyOverlap = false;
	FixPointMesh->SetCollisionProfileName("NoCollision");
	FixPointMesh->SetSimulatePhysics(false);
	FixPointMesh->AttachTo(ScalingMesh);

	RoboMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoboMesh"));
	RoboMesh->bGenerateOverlapEvents = false;
	RoboMesh->bMultiBodyOverlap = false;
	RoboMesh->SetCollisionProfileName("NoCollision");
	RoboMesh->SetSimulatePhysics(false);
	RoboMesh->AttachTo(FixPointMesh);

	RotationVelocity = 0.0f;
	PullVelocity = 500;
	PullAcceleration = 50;
	PushAmount = 150000.0f;

	Type = 999;

	IsPushed = false;
	ReadyForDestroy = false;
	bIsSnaped = false;
	bEmissveEvent = false;

}

// Called when the game starts or when spawned
void ABaseRoboPart::BeginPlay()
{
	Super::BeginPlay();

	//RoboMesh->SetCollisionProfileName("NoCollision");
	
}

// Called every frame
void ABaseRoboPart::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABaseRoboPart::TriggerMagneticStop()
{
	Super::TriggerMagneticStop();

	MagneticMesh->SetEnableGravity(!bIsSnaped);
	MagneticMesh->SetSimulatePhysics(!bIsSnaped);
	magneticMovement->StopMovementImmediately();
	
}

void ABaseRoboPart::TriggerMagneticPush()
{
	Super::TriggerMagneticPush();
	IsPushed = true;
}

