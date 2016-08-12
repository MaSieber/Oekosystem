// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseRoboPart.h"

#include "GameFramework/Actor.h"
#include "RobopartSpawn.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ARobopartSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobopartSpawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobopartSpawn)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobopartSpawn)
	USphereComponent* magneticTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobopartSpawn)
	UParticleSystemComponent* magneticPFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	TSubclassOf<class ABaseRoboPart> SpawnRoboPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	bool IsSpawned;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RobopartSpawn)
	UStaticMeshComponent* SpawnMesh;

	UFUNCTION(BlueprintImplementableEvent, Category = RobopartSpawn)
	void OnSpawn();

	UFUNCTION(BlueprintImplementableEvent, Category = RobopartSpawn)
	void OnDespawn();

private:
	ABaseRoboPart* RoboPart;
	bool bIsDestroying;

	bool bDespawnEventTriggered;

	
};
