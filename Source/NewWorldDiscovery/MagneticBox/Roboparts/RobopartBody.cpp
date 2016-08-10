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

