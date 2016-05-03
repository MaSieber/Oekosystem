// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MagneticBox.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AMagneticBox : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MagneticBox)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MagneticBox)
	UStaticMeshComponent* MagneticMesh;

	// Sets default values for this actor's properties
	AMagneticBox();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = MagneticBox)
	void triggerMagnetic(FVector direction, float force);

	UFUNCTION(BlueprintCallable, Category = MagneticBox)
	void TriggerMagneticStop();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool bPulling;
	float ForceAmount;
	FVector ForceDirection;

	float ForceSeconds;
	float CurrentForceSeconds;
	
};
