// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "TransitionManager.h"

#include "NewWorldDiscoveryCharacter.h"

// Sets default values
ATransitionManager::ATransitionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	transitionLoadTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("transitionLoadTrigger"));
	transitionLoadTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATransitionManager::OnOverlapLoadBegin);
	transitionLoadTrigger->OnComponentEndOverlap.AddDynamic(this, &ATransitionManager::OnOverlapLoadEnd);
	transitionLoadTrigger->bGenerateOverlapEvents = true;
	transitionLoadTrigger->SetSimulatePhysics(false);
	transitionLoadTrigger->AttachTo(SceneComponent);

	transitionUnloadTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("transitionUnloadTrigger"));
	transitionUnloadTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATransitionManager::OnOverlapUnloadBegin);
	transitionUnloadTrigger->OnComponentEndOverlap.AddDynamic(this, &ATransitionManager::OnOverlapUnloadEnd);
	transitionUnloadTrigger->bGenerateOverlapEvents = true;
	transitionUnloadTrigger->SetSimulatePhysics(false);
	transitionUnloadTrigger->AttachTo(SceneComponent);

	bLoadAsync = false;
	bLoadingScreen = false;
	MapName = "FirstLevel";

	bReverse = false;
}

// Called when the game starts or when spawned
void ATransitionManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATransitionManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ATransitionManager::OnOverlapLoadBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		float xDir = 1.0f;
		FVector forward = playerChar->GetActorForwardVector();
		xDir = forward.Y <= 0 ? -1.0f : 1.0f;
		LastInsertDirection = FVector(0.0f, xDir, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("y: %f "), LastInsertDirection.Y);
	}
}
void ATransitionManager::OnOverlapUnloadBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		float xDir = 1.0f;
		FVector forward = playerChar->GetActorForwardVector();
		xDir = forward.Y <= 0 ? -1.0f : 1.0f;
		LastInsertDirection = FVector(0.0f, xDir, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("y: %f "),LastInsertDirection.Y);
	}
}

void ATransitionManager::OnOverlapLoadEnd(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		float xDir = 1.0f;
		FVector forward = playerChar->GetActorForwardVector();
		xDir = forward.Y <= 0 ? -1.0f : 1.0f;
		FVector direction = FVector(0.0f, xDir, 0.0f);

		UE_LOG(LogTemp, Warning, TEXT("y: %f , y2: %f"), direction.Y, LastInsertDirection.Y);
		if (direction == LastInsertDirection)
		{
			int32 increaseIndex = 0;
			//remove from current level
			if (direction.Y < 0.0f)
			{
				increaseIndex = -1;
			}

			OnLoadNewLevel(bReverse,increaseIndex);
			bReverse = !bReverse;
		}
	}
}

void ATransitionManager::OnOverlapUnloadEnd(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		float xDir = 1.0f;
		FVector forward = playerChar->GetActorForwardVector();
		xDir = forward.Y <= 0 ? -1.0f : 1.0f;
		FVector direction = FVector(0.0f, xDir, 0.0f);
		
		UE_LOG(LogTemp, Warning, TEXT("y: %f , y2: %f"), direction.Y, LastInsertDirection.Y);
		if (direction == LastInsertDirection)
		{
			int32 increaseIndex = 0;
			if (direction.Y > 0.0f)
			{
				increaseIndex = 1;
			}
			OnUnloadOldLevel(bReverse, increaseIndex);
			bReverse = !bReverse;
		}
		
	}
}
