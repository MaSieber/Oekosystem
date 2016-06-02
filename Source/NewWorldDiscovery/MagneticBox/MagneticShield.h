// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseMagnetic.h"

#include "GameFramework/Actor.h"
#include "MagneticShield.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AMagneticShield : public ABaseMagnetic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagneticShield();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	virtual void OnOverlap(class AActor* actor, bool bState) override;
	
	
};
