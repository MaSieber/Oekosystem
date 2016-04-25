// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "WorldDiscoveryPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLDDISCOVERY_API AWorldDiscoveryPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AWorldDiscoveryPlayerState();

	UFUNCTION(BlueprintPure, Category = Player)
	int32 GetCurrentEnergy();

	UFUNCTION(BlueprintCallable, Category = Player)
	void AddEnergy(int32 energy);

	UFUNCTION(BlueprintCallable, Category = Player)
	void RemoveEnergy(int32 energy);


private:
	int32 CurrentEnergy;

	
};
