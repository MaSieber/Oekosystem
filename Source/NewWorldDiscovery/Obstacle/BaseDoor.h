// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseDoor.generated.h"

UCLASS()
class NEWWORLDDISCOVERY_API ABaseDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Door)
	USceneComponent* BaseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Door)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Door)
	UStaticMeshComponent* DoorSocketMesh;

	UENUM(BlueprintType)
	enum class eState : uint8
	{
		OPEN = 0,
		CLOSED
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	eState DoorState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	float MaxDoorVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	bool bRepeat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	bool bBackToOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	int32 RepeatCount;

	UFUNCTION(BlueprintCallable, Category = Door)
	void TriggerDoorMove(bool bMove);

	UFUNCTION(BlueprintCallable, Category = Door)
	void OverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = Door)
	void OverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = BaseMagneticEvent)
	void OnOpend();

	UFUNCTION(BlueprintImplementableEvent, Category = BaseMagneticEvent)
	void OnClosed();

private:
	bool bMoveDoor;

	FVector min;
	FVector max;
	FVector Start;
	FVector End;

	FTransform LineTransform;
	
};
