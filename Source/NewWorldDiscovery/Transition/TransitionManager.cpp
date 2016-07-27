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

	transitionLoadTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	transitionLoadTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATransitionManager::OnOverlapBegin);
	transitionLoadTrigger->bGenerateOverlapEvents = true;
	transitionLoadTrigger->SetSimulatePhysics(false);
	transitionLoadTrigger->AttachTo(SceneComponent);

	transitionUnloadTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	transitionUnloadTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATransitionManager::OnOverlapBegin);
	transitionUnloadTrigger->bGenerateOverlapEvents = true;
	transitionUnloadTrigger->SetSimulatePhysics(false);
	transitionUnloadTrigger->AttachTo(SceneComponent);

	bLoadAsync = false;
	bLoadingScreen = false;
	MapName = "FirstLevel";
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

void ATransitionManager::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		OnLoadNewLevel();
	}
}

