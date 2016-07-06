// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerDegree.h"

#include "GameFramework/Actor.h"
#include "PlayerXDegree.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API APlayerXDegree : public APlayerDegree
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerXDegree();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DegreeX)
	USceneComponent* waveSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DegreeX)
	UParticleSystemComponent* magneticWaveSingle;

	
};
