// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TransitionManager.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ATransitionManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransitionManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TransitionManager)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TransitionManager)
	UBoxComponent* transitionTrigger;
	
	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = TransitionManager)
	void OnLoadNewLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TransitionManager)
	bool bLoadAsync;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TransitionManager)
	bool bLoadingScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TransitionManager)
	FString MapName;

};
