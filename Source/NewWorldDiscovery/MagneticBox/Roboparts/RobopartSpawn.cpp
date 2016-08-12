// Fill out your copyright notice in the Description page of Project Settings.



#include "NewWorldDiscovery.h"
#include "RobopartSpawn.h"

#include "../../NewWorldDiscoveryCharacter.h"
#include "../../WorldDiscoveryPlayerState.h"

// Sets default values
ARobopartSpawn::ARobopartSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoboMesh"));
	SpawnMesh->bGenerateOverlapEvents = false;
	SpawnMesh->bMultiBodyOverlap = false;
	SpawnMesh->SetCollisionProfileName("NoCollision");
	SpawnMesh->SetSimulatePhysics(false);
	SpawnMesh->AttachTo(SceneComponent);

	magneticTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	magneticTrigger->bGenerateOverlapEvents = true;
	magneticTrigger->SetSimulatePhysics(false);
	magneticTrigger->SetCollisionProfileName("MapEvent");
	magneticTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARobopartSpawn::OnOverlapBegin);
	magneticTrigger->AttachTo(SpawnMesh);

	magneticPFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WaveParticle"));
	magneticPFX->AttachTo(magneticTrigger);

	bIsDestroying = false;
	bDespawnEventTriggered = false;
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


	if (RoboPart != nullptr)
	{
		if (RoboPart->ReadyForDestroy)
		{
			RoboPart->K2_DestroyActor();
			IsSpawned = false;
			RoboPart = nullptr;
		}
		else if (RoboPart->IsPushed && bIsDestroying == false)
		{
			bIsDestroying = true;
			RoboPart->OnDestroying();
		}
		else if (!bDespawnEventTriggered && RoboPart->IsGrabbed()) {
			OnDespawn();
			bDespawnEventTriggered = true;
		}

	}

}

void ARobopartSpawn::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsSpawned)
		return;

	ANewWorldDiscoveryCharacter *character = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (character)
	{
		AWorldDiscoveryPlayerState *state = Cast<AWorldDiscoveryPlayerState>(character->PlayerState);

		if (state->GetRoboParts() < 31)
			return;

		if (SpawnRoboPart)
		{
			character->GetMovementComponent()->StopMovementImmediately();

			FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
			SpawnParameters.bNoFail = true;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

			FVector ActorPos = magneticTrigger->GetComponentLocation();
			ActorPos.Z += 30.0f;
			RoboPart = GetWorld()->SpawnActor<ABaseRoboPart>(SpawnRoboPart, ActorPos, Rotation, SpawnParameters);
			if (RoboPart)
			{
				RoboPart->MagneticMesh->bGenerateOverlapEvents = true;
				RoboPart->MagneticMesh->SetSimulatePhysics(false);
				RoboPart->MagneticMesh->SetEnableGravity(true);
				IsSpawned = true;
				bIsDestroying = false;
				bDespawnEventTriggered = false;
				OnSpawn();
			}
		}
	}
}
