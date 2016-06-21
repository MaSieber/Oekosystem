// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlayerDegree.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API APlayerDegree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerDegree();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Degree360)
	USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Degree360)
	USphereComponent* magneticTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Degree360)
	UParticleSystemComponent* magneticWave;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	ACharacter* parentCharacter;

private:
	float RotationRate;

};
