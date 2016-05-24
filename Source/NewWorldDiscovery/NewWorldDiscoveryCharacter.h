// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "MagneticBox/BaseMagnetic.h"
#include "GameFramework/Character.h"
#include "NewWorldDiscoveryCharacter.generated.h"

UCLASS(config=Game)
class ANewWorldDiscoveryCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MagneticCollider)
	USphereComponent* magneticTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticBox)
	AActor* PulledObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MagneticBox)
	TArray<ABaseMagnetic*> CreatedBoxes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MagneticBox)
	TArray<ABaseMagnetic*> CreatedBalls;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MagneticBox)
	TArray<ABaseMagnetic*> CreatedPyramides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticBox)
	int32 MaxBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticBox)
	int32 MaxBalls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticBox)
	int32 MaxPyramides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticBox)
	TSubclassOf<class ABaseMagnetic> MagneticBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticBox)
	TSubclassOf<class ABaseMagnetic> MagneticBall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MagneticBox)
	TSubclassOf<class ABaseMagnetic> MagneticPyramide;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	void RotateAround(float Val);

	void CreateMagneticBox();
	void CreateMagneticBall();
	void CreateMagneticPyramide();


public:
	ANewWorldDiscoveryCharacter();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = MagneticBox)
	AActor* GetPulledObject();

};
