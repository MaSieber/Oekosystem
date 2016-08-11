// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "RobopartBody.h"


// Sets default values
ARobopartBody::ARobopartBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	HeadPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HeadPoint"));
	HeadPoint->AttachTo(RoboMesh);

	RightArmPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightArmPoint"));
	RightArmPoint->AttachTo(RoboMesh);

	LeftArmPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftArmPoint"));
	LeftArmPoint->AttachTo(RoboMesh);

	LeftLegPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftLegPoint"));
	LeftLegPoint->AttachTo(RoboMesh);

	RightLegPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightLegPoint"));
	RightLegPoint->AttachTo(RoboMesh);

	HeadPointCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadPointCollider"));
	HeadPointCollider->bGenerateOverlapEvents = true;
	HeadPointCollider->SetSimulatePhysics(false);
	HeadPointCollider->SetCollisionProfileName("ObjectMagnet");
	HeadPointCollider->AttachTo(HeadPoint);

	RightArmPointCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArmPointCollider"));
	RightArmPointCollider->bGenerateOverlapEvents = true;
	RightArmPointCollider->SetSimulatePhysics(false);
	RightArmPointCollider->SetCollisionProfileName("ObjectMagnet");
	RightArmPointCollider->AttachTo(RightArmPoint);

	LeftArmPointCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArmPointCollider"));
	LeftArmPointCollider->bGenerateOverlapEvents = true;
	LeftArmPointCollider->SetSimulatePhysics(false);
	LeftArmPointCollider->SetCollisionProfileName("ObjectMagnet");
	LeftArmPointCollider->AttachTo(LeftArmPoint);

	LeftLegPointCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLegPointCollider"));
	LeftLegPointCollider->bGenerateOverlapEvents = true;
	LeftLegPointCollider->SetSimulatePhysics(false);
	LeftLegPointCollider->SetCollisionProfileName("ObjectMagnet");
	LeftLegPointCollider->AttachTo(LeftLegPoint);

	RightLegPointCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLegPointCollider"));
	RightLegPointCollider->bGenerateOverlapEvents = true;
	RightLegPointCollider->SetSimulatePhysics(false);
	RightLegPointCollider->SetCollisionProfileName("ObjectMagnet");
	RightLegPointCollider->AttachTo(RightLegPoint);


}

// Called when the game starts or when spawned
void ARobopartBody::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobopartBody::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

