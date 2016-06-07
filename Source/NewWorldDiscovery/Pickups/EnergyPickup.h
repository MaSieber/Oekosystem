// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnergyPickup.generated.h"


UCLASS()
class NEWWORLDDISCOVERY_API AEnergyPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnergyPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when something enters the sphere component
	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = BatteryStationEvent)
	void OnPickup();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = EnergyPickup)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = EnergyPickup)
	UBoxComponent* PickupEventCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnergyPickup)
	int32 ProvidingRessource;
};
