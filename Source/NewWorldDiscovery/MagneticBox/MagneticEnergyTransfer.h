// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseMagnetic.h"

#include "GameFramework/Actor.h"
#include "MagneticEnergyTransfer.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AMagneticEnergyTransfer : public ABaseMagnetic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagneticEnergyTransfer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintImplementableEvent, Category = BaseMagneticEvent)
	void OnPoweringUp();

	UFUNCTION(BlueprintImplementableEvent, Category = BaseMagneticEvent)
	void OnPoweringDown();

	UFUNCTION()
	virtual void OnOverlap(class AActor* actor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticTransfer)
	float MaxEnergy;

	UFUNCTION()
	void triggerPoweringDown();
};
