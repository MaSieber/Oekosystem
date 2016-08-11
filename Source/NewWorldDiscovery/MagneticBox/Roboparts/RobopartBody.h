// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseRoboPart.h"

#include "GameFramework/Actor.h"
#include "RobopartBody.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ARobopartBody : public ABaseRoboPart
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobopartBody();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RobopartBody)
	USceneComponent* HeadPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RobopartBody)
	USceneComponent* LeftArmPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RobopartBody)
	USceneComponent* RightArmPoint;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RobopartBody)
	USceneComponent* LeftLegPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RobopartBody)
	USceneComponent* RightLegPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	UStaticMeshComponent* HeadPointCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	UStaticMeshComponent* LeftArmPointCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	UStaticMeshComponent* RightArmPointCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	UStaticMeshComponent* LeftLegPointCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	UStaticMeshComponent* RightLegPointCollider;
	
};
