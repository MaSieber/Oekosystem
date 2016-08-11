// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MagnetBlueprintPickup.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AMagnetBlueprintPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagnetBlueprintPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = AbilityPickup)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = EnergyPickup)
	UBoxComponent* PickupEventCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = AbilityPickup)
	UStaticMeshComponent* PickupMesh;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UENUM(BlueprintType)
	enum class EAbility : uint8
	{
		NONE = 0,
		BOX = 1,
		BALL = 2,
		SHIELD = 4,
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityPickup)
	EAbility AbilityType;

	UFUNCTION(BlueprintImplementableEvent, Category = BaseMagneticEvent)
	void OnDestroying();

};
