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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	TSubclassOf<class AActor> ObjectToSnap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	bool bLeftActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	bool bRightActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	int SnapDelay;

	UFUNCTION()
	void SpawnMagnet();

	UFUNCTION()
	void SpawnMagnet2();

	UFUNCTION()
	virtual void DestroyMagneticObject();
	


protected:
	AObjectMagnet* objectMagnet;
	AObjectMagnet* objectMagnet2;
	bool bMagneticEffect;
	bool bStatic;

	UChildActorComponent* MagneticObject;

	bool IsDestroyed;
	bool bSnapDelay;

private:
	bool bBeginDestroy;

};
