// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Obstacle/BaseDoor.h"

#include "GameFramework/Actor.h"
#include "ElectricDoor.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AElectricDoor : public ABaseDoor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElectricDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
