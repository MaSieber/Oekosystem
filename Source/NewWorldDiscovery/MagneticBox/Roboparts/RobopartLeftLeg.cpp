// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "RobopartLeftLeg.h"


// Sets default values
ARobopartLeftLeg::ARobopartLeftLeg()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobopartLeftLeg::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobopartLeftLeg::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

