// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseDoor.h"

#include "GameFramework/Actor.h"
#include "Switch.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ASwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Switch)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Switch)
	UStaticMeshComponent* SwitchMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Switch)
	UStaticMeshComponent* SwitchSocketMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Switch)
	UBoxComponent* BoxCollisionTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Switch)
	TArray<ABaseDoor*> Doors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Switch)
	TArray<AActor*> BackgroundActors;
};
