// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MagneticEnergyTransfer.h"

#include "../Electric/ElectricProvider.h"

// Sets default values
AMagneticEnergyTransfer::AMagneticEnergyTransfer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMagneticEnergyTransfer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagneticEnergyTransfer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMagneticEnergyTransfer::OnOverlap(class AActor* actor)
{
	Super::OnOverlap(actor);

	UE_LOG(LogTemp,Warning,TEXT("AMagneticEnergyTransfer - OnOverlap"));

	AElectricProvider *provider = Cast<AElectricProvider>(actor);
	if (provider)
		OnPoweringUp();
}

void AMagneticEnergyTransfer::triggerPoweringDown()
{
	OnPoweringDown();
}

