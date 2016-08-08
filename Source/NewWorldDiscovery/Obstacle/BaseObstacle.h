// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ObjectMagnet/ObjectMagnet.h"

#include "GameFramework/Actor.h"
#include "BaseObstacle.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ABaseObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseObstacle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	TSubclassOf<class AObjectMagnet> MagnetAbility;

	UFUNCTION()
	void SpawnMagnet();

	UFUNCTION()
	virtual void DestroyMagneticObject();
	
protected:
	AObjectMagnet* objectMagnet;
	bool bMagneticEffect;
	bool bStatic;
	UStaticMeshComponent* MagneticObject;
	bool IsDestroyed;

private:
	bool bBeginDestroy;

};
