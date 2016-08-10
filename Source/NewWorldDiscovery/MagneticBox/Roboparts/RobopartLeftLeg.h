// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseRoboPart.h"

#include "GameFramework/Actor.h"
#include "RobopartLeftLeg.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ARobopartLeftLeg : public ABaseRoboPart
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobopartLeftLeg();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
