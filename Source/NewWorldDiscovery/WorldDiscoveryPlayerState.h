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

	UFUNCTION(BlueprintPure, Category = Player)
	int32 GetCurrentRessource();

	UFUNCTION(BlueprintCallable, Category = Player)
	void AddRessource(int32 ressource);

	UFUNCTION(BlueprintCallable, Category = Player)
	void RemoveRessource(int32 ressource);

	UFUNCTION(BlueprintCallable, Category = Player)
	void AddAbility(int32 ability);

	UFUNCTION(BlueprintCallable, Category = Player)
	bool HasBallAbility();

	UFUNCTION(BlueprintCallable, Category = Player)
	bool HasBoxAbility();

	UFUNCTION(BlueprintCallable, Category = Player)
	bool HasShieldAbility();

	UENUM(BlueprintType)
	enum class eRoboParts : uint8
	{
		NONE = 0,
		HEAD = 1,
		ARM_LEFT = 2,
		ARM_RIGHT = 4,
		LEG_LEFT = 8,
		LEG_RIGHT = 16
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	eRoboParts RoboParts;

	UFUNCTION(BlueprintCallable, Category = Player)
	void AddRoboPart(int32 part);

	UFUNCTION(BlueprintCallable, Category = Player)
	int32 GetRoboParts();

private:
	int32 CurrentEnergy;
	int32 CurrentRessource;

	int32 CollectedParts;

	int32 Abilitys;
	
};
