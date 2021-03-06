// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "Switch.h"

#include "../WorldDiscoveryPlayerController.h"
#include "../NewWorldDiscoveryCharacter.h"

// Sets default values
ASwitch::ASwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	SwitchMesh->bGenerateOverlapEvents = true;
	SwitchMesh->AttachTo(BaseComponent);

	BoxCollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionTrigger"));
	BoxCollisionTrigger->bGenerateOverlapEvents = true;
	BoxCollisionTrigger->SetCollisionProfileName("PlatformTrigger");
	BoxCollisionTrigger->AttachTo(SwitchMesh);

	SwitchSocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchSocketMesh"));
	SwitchSocketMesh->bGenerateOverlapEvents = false;
	SwitchSocketMesh->AttachTo(BaseComponent);

}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwitch::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

