// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "BaseObstacle.h"


// Sets default values
ABaseObstacle::ABaseObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objectMagnet = nullptr;
	bStatic = false;
	MagneticObject = nullptr;
	IsDestroyed = true;
	bBeginDestroy = false;
}

// Called when the game starts or when spawned
void ABaseObstacle::BeginPlay()
{
	Super::BeginPlay();

	SpawnMagnet();
	
}

// Called every frame
void ABaseObstacle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	int state = 0;
	if (MagneticObject != nullptr && MagneticObject->IsBeingDestroyed())
	{
		state = 1;
		UE_LOG(LogTemp, Warning, TEXT("%d"), state);
	}
		

	

	if (bBeginDestroy && MagneticObject != nullptr && MagneticObject->IsBeingDestroyed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finishing Destroying"));
		MagneticObject = nullptr;
		IsDestroyed = true;
		bBeginDestroy = false;
		
	}

}

void ABaseObstacle::SpawnMagnet()
{
	if (MagnetAbility)
	{
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.bNoFail = true;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

		FVector ActorPos = GetActorLocation();

		objectMagnet = GetWorld()->SpawnActor<AObjectMagnet>(MagnetAbility, ActorPos, Rotation, SpawnParameters);
		if (objectMagnet)
		{
			objectMagnet->magneticTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			objectMagnet->magneticTrigger->bGenerateOverlapEvents = true;
			objectMagnet->magneticWave->Activate();
			bMagneticEffect = true;
		}
	}
}

void ABaseObstacle::DestroyMagneticObject()
{
	UE_LOG(LogTemp,Warning,TEXT("DestroyMagneticObject"));
	if (MagneticObject != nullptr)
	{
		bBeginDestroy = true;
		MagneticObject->UnregisterComponent();
		MagneticObject->DestroyComponent();

		IsDestroyed = false;
		UE_LOG(LogTemp, Warning, TEXT("Destroy triggered"));
	}	
}


