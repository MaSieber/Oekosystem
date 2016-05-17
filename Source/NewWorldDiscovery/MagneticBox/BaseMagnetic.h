// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseMagnetic.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ABaseMagnetic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseMagnetic();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BaseMagnetic)
	UStaticMeshComponent* MagneticMesh;

	UFUNCTION(BlueprintCallable, Category = BaseMagnetic)
	void triggerMagnetic(FVector direction, float force);

	UFUNCTION(BlueprintCallable, Category = BaseMagnetic)
	void TriggerMagneticStop();

	UFUNCTION(BlueprintCallable, Category = BaseMagnetic)
	void TriggerMagneticPush();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationAmplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float PushAmount;

protected:
	void Accelerate(float DeltaTime);

	float CurrentVelocity;

	float ForceAmount;
	FVector ForceDirection;

	float ForceSeconds;
	float CurrentForceSeconds;

	FVector TargetLocation;

	enum ePulling
	{
		NONE = 0,
		PULLING,
		FOLLOWING,
		PUSHING
	};
	int PullingType;
	
};
