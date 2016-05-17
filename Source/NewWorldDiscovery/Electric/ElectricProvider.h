// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ElectricProvider.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AElectricProvider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElectricProvider();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricProvider)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricProvider)
	UStaticMeshComponent* ElectricMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricProvider)
	UStaticMeshComponent* ElectricSocketMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricProvider)
	UBoxComponent* BoxCollisionTrigger;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ElectricProvider)
	UParticleSystemComponent* ElectricParticle;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
