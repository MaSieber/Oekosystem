// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovingPlatform.h"

#include "GameFramework/Actor.h"
#include "MovingPlatformReset.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AMovingPlatformReset : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatformReset();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatformReset)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatformReset)
	UStaticMeshComponent* ResetMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatformReset)
	UStaticMeshComponent* ResetSocketMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatformReset)
	UBoxComponent* BoxCollisionTrigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Platforms)
	TArray<AMovingPlatform*> Platforms;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = MovingPlatform)
	void OnReset();

private:
	void Reset();

};
