// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MagneticMovementComponent.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseMagnetic)
	UProjectileMovementComponent *magneticMovement;
	
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BaseMagnetic)
	//UMovementComponent *MovementComponent;

	UFUNCTION(BlueprintCallable, Category = BaseMagnetic)
	void triggerMagnetic(FVector direction, float force);

	UFUNCTION(BlueprintCallable, Category = BaseMagnetic)
	void TriggerMagneticStop();

	UFUNCTION(BlueprintCallable, Category = BaseMagnetic)
	void TriggerMagneticPush();

	UFUNCTION(BlueprintImplementableEvent, Category = BaseMagneticEvent)
	void OnCreate();

	UFUNCTION(BlueprintImplementableEvent, Category = BaseMagneticEvent)
	void OnDestroying();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlap(class AActor* actor,bool bState);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	ACharacter* parentCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float PullAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float PullVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationAroundVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationFollowVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationPercentDistanceVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationAmplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float RotationFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	float PushAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseMagnetic)
	bool bIgnoreMagnetic;

	UFUNCTION()
	void SetRotationRate(float Val);

	UFUNCTION()
	void TriggerDestroy(bool bInstant);

	UFUNCTION()
	bool IsInteractible();

	UFUNCTION()
	void SetNewMassScale(const float& Scale);

	UFUNCTION()
	void Reset();

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

	float RotationCurrent;
	float RotationRate;

	float StaticXPos;
	
	bool bDestroying;
	bool bIsDestroyed;

	FVector OriginLocation;
	FRotator OriginRotation;

	bool bStop;

	int counter;

private:
	FVector OldTarget;
};
