// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "RobopartRightArm.h"


// Sets default values
ARobopartRightArm::ARobopartRightArm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobopartRightArm::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobopartRightArm::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

