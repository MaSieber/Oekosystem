// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "WorldDiscoveryGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLDDISCOVERY_API UWorldDiscoveryGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UWorldDiscoveryGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	/** Increment this value in any map, change map, and notice it persists! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Map)
	int32 CurrentMapIndex;
	
	
};
