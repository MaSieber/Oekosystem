// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MagneticShield.h"

// Sets default values
AMagneticShield::AMagneticShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMagneticShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagneticShield::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMagneticShield::OnOverlap(class AActor* actor, bool bState)
{
	Super::OnOverlap(actor,bState);

}