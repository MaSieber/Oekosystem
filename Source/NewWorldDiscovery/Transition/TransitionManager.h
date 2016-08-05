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
	UBoxComponent* transitionLoadTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TransitionManager)
	UBoxComponent* transitionUnloadTrigger;

	UFUNCTION()
	void OnOverlapLoadBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapUnloadBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapLoadEnd(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapUnloadEnd(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintImplementableEvent, Category = TransitionManager)
	void OnLoadNewLevel(bool Reverse, int32 xDir);

	UFUNCTION(BlueprintImplementableEvent, Category = TransitionManager)
	void OnUnloadOldLevel(bool Reverse, int32 xDir);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TransitionManager)
	bool bLoadAsync;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TransitionManager)
	bool bLoadingScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TransitionManager)
	bool bReverse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TransitionManager)
	FString MapName;

private:
	FVector LastInsertDirection;
};
