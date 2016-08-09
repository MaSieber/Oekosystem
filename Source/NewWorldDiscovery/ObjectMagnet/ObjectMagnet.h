// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ObjectMagnet.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AObjectMagnet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectMagnet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	UStaticMeshComponent* magneticTrigger;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	UStaticMeshComponent* UnrealFickDich;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ObjectMagnet)
	UParticleSystemComponent* magneticWave;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ObjectMagnet)
	AActor* parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ObjectMagnet)
	float PullingVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ObjectMagnet)
	float Radius;
	


	
};
