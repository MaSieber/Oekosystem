// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "Player360Degree.h"


// Sets default values
APlayer360Degree::APlayer360Degree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayer360Degree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer360Degree::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

