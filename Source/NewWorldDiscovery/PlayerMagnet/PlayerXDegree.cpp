// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "PlayerXDegree.h"


// Sets default values
APlayerXDegree::APlayerXDegree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	magneticTrigger->RelativeLocation.Y = -200.0f;

	UE_LOG(LogTemp, Warning, TEXT("const RelativeLocation %f %f %f"), magneticTrigger->RelativeLocation.X, magneticTrigger->RelativeLocation.Y, magneticTrigger->RelativeLocation.Z);

}

// Called when the game starts or when spawned
void APlayerXDegree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerXDegree::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

