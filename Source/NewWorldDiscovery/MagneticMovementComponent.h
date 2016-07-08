// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MagneticMovementComponent.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API UMagneticMovementComponent : public UMovementComponent
{

	GENERATED_UCLASS_BODY()

protected:

	//UMagneticMovementComponent(const FObjectInitializer& ObjectInitializer);

	//Init
	virtual void InitializeComponent() override;

	//Tick
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

};
