// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "ElectricCost.h"

#include "MagneticBox/MagneticEnergyTransfer.h"

// Sets default values
AElectricCost::AElectricCost()
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
	BoxCollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AElectricCost::OnOverlapBegin);
	BoxCollisionTrigger->AttachTo(ElectricMesh);

	ElectricSocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElectricSocketMesh"));
	ElectricSocketMesh->bGenerateOverlapEvents = false;
	ElectricSocketMesh->AttachTo(BaseComponent);

	ElectricParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ElectricParticle"));
	ElectricParticle->AttachTo(BaseComponent);
	ElectricParticle->bAutoActivate = true;

	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticle"));
	ExplosionParticle->AttachTo(BaseComponent);
	ExplosionParticle->bAutoActivate = false;

}

// Called when the game starts or when spawned
void AElectricCost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElectricCost::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AElectricCost::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMagneticEnergyTransfer *energyTransfer = Cast<AMagneticEnergyTransfer>(OtherActor);
	if (energyTransfer && energyTransfer->GetChargeState())
	{
		UE_LOG(LogTemp, Warning, TEXT("AElectricCost - OnOverlapBegin"));

		if (Doors.Num() > 0)
		{
			for (int i = 0; i < Doors.Num(); i++)
			{
				ABaseDoor *door = Cast<ABaseDoor>(Doors[i]);
				if (door)
				{
					door->TriggerDoorMove(true);
				}
			}
		}
		ExplosionParticle->Activate();
		ElectricParticle->Deactivate();

		OnExploding();
	}
}
