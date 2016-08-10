// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "RobopartHead.h"


// Sets default values
ARobopartHead::ARobopartHead()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobopartHead::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobopartHead::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

