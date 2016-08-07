// Fill out your copyright notice in the Description page of Project Settings.

#include "../../WorldDiscoveryPlayerState.h"

#include "NewWorldDiscovery.h"
#include "RobopartSpawn.h"


// Sets default values
ARobopartSpawn::ARobopartSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	magneticTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	magneticTrigger->bGenerateOverlapEvents = true;
	magneticTrigger->SetSimulatePhysics(false);
	magneticTrigger->SetCollisionProfileName("MapEvent");
	magneticTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARobopartSpawn::OnOverlapBegin);
	magneticTrigger->AttachTo(SceneComponent);

	magneticPFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WaveParticle"));
	magneticPFX->AttachTo(magneticTrigger);

}

// Called when the game starts or when spawned
void ARobopartSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobopartSpawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ARobopartSpawn::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SpawnRoboPart)
	{
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.bNoFail = true;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

		FVector ActorPos = GetActorLocation();

		RoboPart = GetWorld()->SpawnActor<ABaseRoboPart>(SpawnRoboPart, ActorPos, Rotation, SpawnParameters);
		if (RoboPart)
		{
			RoboPart->MagneticMesh->SetSimulatePhysics(false);
			RoboPart->MagneticMesh->SetEnableGravity(false);
		}
	}
}
