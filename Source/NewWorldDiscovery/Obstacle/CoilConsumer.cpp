// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "CoilConsumer.h"

#include "../MagneticBox/MagneticEnergyTransfer.h"


// Sets default values
ACoilConsumer::ACoilConsumer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	CoilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoilMesh"));
	CoilMesh->bGenerateOverlapEvents = false;
	CoilMesh->AttachTo(BaseComponent);

	CollisionTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionTrigger"));
	CollisionTrigger->bGenerateOverlapEvents = true;
	CollisionTrigger->SetCollisionProfileName("PlatformTrigger");
	CollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACoilConsumer::OnOverlapBegin);
	CollisionTrigger->AttachTo(CoilMesh);

}

// Called when the game starts or when spawned
void ACoilConsumer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoilConsumer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ACoilConsumer::OnOverlapBegin(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMagneticEnergyTransfer *transfer = Cast<AMagneticEnergyTransfer>(OtherActor);
	if (transfer)
	{
		transfer->triggerPoweringDown();
	}
}

