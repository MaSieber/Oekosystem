// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "WorldDiscoveryPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLDDISCOVERY_API AWorldDiscoveryPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

};
