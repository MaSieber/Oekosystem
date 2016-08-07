// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseMagnetic.h"
#include "RobopartSpawn.h"

#include "GameFramework/Actor.h"
#include "BaseRoboPart.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ABaseRoboPart : public ABaseMagnetic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseRoboPart();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BaseRoboParts)
	USceneComponent* BaseComponent;

};
