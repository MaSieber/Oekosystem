// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "NewWorldDiscoveryGameMode.generated.h"

UCLASS(minimalapi)
class ANewWorldDiscoveryGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ANewWorldDiscoveryGameMode();


	UFUNCTION(BlueprintPure, Category = Player)
	int32 GetEnergyLimit();

	UFUNCTION(BlueprintPure, Category = Player)
	int32 GetRessourceLimit();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	int32 EnergyLimit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	int32 RessourceLimit;

};



