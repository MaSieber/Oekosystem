// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ElectricDoor.h"

#include "GameFramework/Actor.h"
#include "ElectricCost.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AElectricCost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElectricCost();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricCost)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricCost)
	UStaticMeshComponent* ElectricMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricCost)
	UStaticMeshComponent* ElectricSocketMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricCost)
	UBoxComponent* BoxCollisionTrigger;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricCost)
	UParticleSystemComponent* ElectricParticle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricCost)
	UParticleSystemComponent* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ElectricCost)
	TArray<ABaseDoor*> Doors;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = ElectricCostEvent)
	void OnExploding();
	
};
