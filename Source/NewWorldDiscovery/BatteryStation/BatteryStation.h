// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BatteryStation.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ABatteryStation : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BatteryStation)
	float EnergyRefuelTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BatteryStation)
	int32 ProvidingEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BatteryStation)
	bool bInstantEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BatteryStation)
	bool bCheckpoint;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = BatteryStation)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BatteryStation)
	UStaticMeshComponent* BatteryMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BatteryStation)
	UBoxComponent* BatteryEventCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Checkpoint)
	TArray<AActor*> ObjectsToReset;

	// Sets default values for this actor's properties
	ABatteryStation();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when something enters the sphere component
	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = BatteryStationEvent)
	void OnEnergyRise();

	UFUNCTION(BlueprintImplementableEvent, Category = BatteryStationEvent)
	void OnEnergyFall();

	UFUNCTION()
	void Reset();

private:
	int32 CurrentEnergy;
	bool bEnergyRefuel;
	float CurrentEnergyRefuelTime;

	
};
