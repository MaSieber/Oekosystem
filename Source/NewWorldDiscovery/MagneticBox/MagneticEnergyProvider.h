// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseMagnetic.h"

#include "GameFramework/Actor.h"
#include "MagneticEnergyProvider.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AMagneticEnergyProvider : public ABaseMagnetic
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMagneticEnergyProvider();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticProvider)
	float MaxEnergy;
};
