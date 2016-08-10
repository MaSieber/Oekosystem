// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseObstacle.h"

#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API AMovingPlatform : public ABaseObstacle
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = MovingPlatform)
	void OverlapBegin(int32 Collider, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = MovingPlatform)
	void OverlapEnd(int32 Collider, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UENUM(BlueprintType)
	enum class eTypeDirection : uint8
	{
		HORIZONTAL = 0,
		VERTICAL
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovingPlatform)
	eTypeDirection Type;

	UENUM(BlueprintType)
	enum class eInitialDirection : uint8
	{
		START = 0,
		END
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovingPlatform)
	eInitialDirection InitialDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovingPlatform)
	bool bActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovingPlatform)
	bool bFixedSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovingPlatform)
	float MaxPlatformVelocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovingPlatform)
	float Acceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatform)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatform)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatform)
	UStaticMeshComponent* PlatformLineMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatform)
	UStaticMeshComponent* PlatformEnergySocketMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatform)
	UStaticMeshComponent* PlatformEnergySocketMeshRight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatform)
	UBoxComponent* BoxCollisionTrigger;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MovingPlatform)
	UBoxComponent* BoxCollisionTrigger2;

	UFUNCTION(BlueprintCallable, Category = MovingPlatform)
	void TriggerPlatform(bool bActiveState);

	UFUNCTION(BlueprintImplementableEvent, Category = MovingPlatform)
	void OnTriggerPlatform(bool bActiveState);

	UFUNCTION()
	void SetStoringEnergy(uint32 energy);

	UFUNCTION(BlueprintCallable, Category = MovingPlatform)
	void ResetPlatform(bool bInstant);

	UFUNCTION()
	virtual void DestroyMagneticObject() override;

private:
	uint32 StoringEnergy;
	float CurrentVelocity;

	void Accelerate(float DeltaTime);

	float StartDirection;
	float MoveDirection;

	void DirectionSwitch(float current, float start, float end);
	void SpawnChildActor(AActor* box);
	void EnablePlatform();

	FVector RelOriginPosition;
	eInitialDirection OriginDirection;
	bool bOriginActive;

	AActor *actor;
	AActor *CollisionActor;
	int32 ColliderType;

	bool bRestAnim;
	bool bStarted;
	
};
