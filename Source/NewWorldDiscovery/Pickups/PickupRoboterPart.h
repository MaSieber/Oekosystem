// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickupRoboterPart.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API APickupRoboterPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupRoboterPart();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PickupRoboterPart)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PickupRoboterPart)
	UBoxComponent* PickupEventCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PickupRoboterPart)
	UStaticMeshComponent *RoboPartMesh;

	
};
