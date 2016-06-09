// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "ElectricProvider.h"

#include "MagneticBox/MagneticEnergyTransfer.h"

// Sets default values
AElectricProvider::AElectricProvider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	ElectricMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElectricMesh"));
	ElectricMesh->bGenerateOverlapEvents = true;
	ElectricMesh->AttachTo(BaseComponent);

	BoxCollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionTrigger"));
	BoxCollisionTrigger->bGenerateOverlapEvents = true;
	BoxCollisionTrigger->SetCollisionProfileName("PlatformTrigger");
	BoxCollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AElectricProvider::OnOverlapBegin);
	BoxCollisionTrigger->OnComponentEndOverlap.AddDynamic(this, &AElectricProvider::OnOverlapEnd);
	BoxCollisionTrigger->AttachTo(ElectricMesh);

	ElectricSocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElectricSocketMesh"));
	ElectricSocketMesh->bGenerateOverlapEvents = false;
	ElectricSocketMesh->AttachTo(BaseComponent);

	ElectricParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ElectricParticle"));
	ElectricParticle->AttachTo(BaseComponent);


}

// Called when the game starts or when spawned
void AElectricProvider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElectricProvider::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void AElectricProvider::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMagneticEnergyTransfer *energyTransfer = Cast<AMagneticEnergyTransfer>(OtherActor);
	if (energyTransfer)
	{
		UE_LOG(LogTemp, Warning, TEXT("AElectricProvider - OnOverlapBegin"));
	}
}

void AElectricProvider::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMagneticEnergyTransfer *energyTransfer = Cast<AMagneticEnergyTransfer>(OtherActor);
	if (energyTransfer)
	{
		UE_LOG(LogTemp, Warning, TEXT("AElectricProvider - OnOverlapEnd"));
	}
}
