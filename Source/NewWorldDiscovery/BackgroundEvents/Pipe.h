// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pipe.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API APipe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APipe();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pipe)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pipe)
	UStaticMeshComponent *PipeMesh;
	
	
};
