// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MagneticBox.h"


// Sets default values
AMagneticBox::AMagneticBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Type = 1;

}

// Called when the game starts or when spawned
void AMagneticBox::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Warning,TEXT("BeginPlay - MagneticBox"));

	if (MagneticMesh->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("Physic Enabled"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Physic Disabled"));
	}

	if (MagneticMesh->IsGravityEnabled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Gravity Enabled"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Gravity Disabled"));
	}

	if (MagneticMesh->bGenerateOverlapEvents)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Enabled"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Disabled"));
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *MagneticMesh->GetCollisionProfileName().ToString());
}

void AMagneticBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Todo: better solution: workaround
	if (bForceShit == true)
	{
		MagneticMesh->SetSimulatePhysics(false);
		MagneticMesh->SetEnableGravity(false);
	}
}
