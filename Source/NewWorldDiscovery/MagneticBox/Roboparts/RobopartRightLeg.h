// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseRoboPart.h"

#include "GameFramework/Actor.h"
#include "RobopartRightLeg.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ARobopartRightLeg : public ABaseRoboPart
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobopartRightLeg();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
