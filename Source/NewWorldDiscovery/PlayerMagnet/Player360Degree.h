// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerDegree.h"

#include "GameFramework/Actor.h"
#include "Player360Degree.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API APlayer360Degree : public APlayerDegree
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer360Degree();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
};