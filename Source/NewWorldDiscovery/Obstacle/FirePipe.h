// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FirePipe.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AFirePipe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirePipe();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirePipe)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirePipe)
	UStaticMeshComponent* FireMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirePipe)
	UCapsuleComponent* CollisionTrigger;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirePipe)
	UParticleSystemComponent *fireParticle;
	
	UFUNCTION()
	void SetCanDoDamage(bool bDamage);

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bCanDoDamage;
};
