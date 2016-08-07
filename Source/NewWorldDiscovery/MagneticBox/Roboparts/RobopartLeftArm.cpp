// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "RobopartLeftArm.h"


// Sets default values
ARobopartLeftArm::ARobopartLeftArm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobopartLeftArm::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobopartLeftArm::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

