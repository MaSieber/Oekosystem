// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "FirePipe.h"

#include "../NewWorldDiscoveryCharacter.h"
#include "../MagneticBox/BaseMagnetic.h"
#include "../MagneticBox/MagneticShield.h"

// Sets default values
AFirePipe::AFirePipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoilMesh"));
	FireMesh->bGenerateOverlapEvents = false;
	FireMesh->AttachTo(BaseComponent);

	CollisionTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionTrigger"));
	CollisionTrigger->bGenerateOverlapEvents = true;
	CollisionTrigger->SetCollisionProfileName("PlatformTrigger");
	CollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFirePipe::OnOverlapBegin);
	CollisionTrigger->OnComponentEndOverlap.AddDynamic(this, &AFirePipe::OnOverlapEnd);
	CollisionTrigger->AttachTo(FireMesh);

	fireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	fireParticle->bGenerateOverlapEvents = false;
	fireParticle->bAutoActivate = true;
	fireParticle->AttachTo(FireMesh);

	bCanBeDamaged = true;

	character = nullptr;
}

// Called when the game starts or when spawned
void AFirePipe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirePipe::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	float rad = CollisionTrigger->GetUnscaledCapsuleRadius();
	CollisionTrigger->SetCapsuleRadius(rad + FMath::Sin(DeltaTime) / 10.0f);

	if (bCanBeDamaged)
	{
		ANewWorldDiscoveryCharacter* playerCharacter = Cast<ANewWorldDiscoveryCharacter>(character);
		if (playerCharacter)
		{
			playerCharacter->Reset();
			character = nullptr;
		}
	}
}

void AFirePipe::SetCanDoDamage(bool bDamage)
{
	this->bCanBeDamaged = bDamage;
}

void AFirePipe::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseMagnetic* baseMagnetic = Cast<ABaseMagnetic>(OtherActor);
	if (baseMagnetic)
	{
		SetCanDoDamage(false);
		AMagneticShield* magneticShield = Cast<AMagneticShield>(baseMagnetic);
		if (!magneticShield)
		{
			baseMagnetic->TriggerDestroy(false);
		}
	}
	ANewWorldDiscoveryCharacter* playerCharacter = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerCharacter)
	{
		character = playerCharacter;
	}
}

void AFirePipe::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANewWorldDiscoveryCharacter* playerCharacter = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerCharacter)
	{
		character = nullptr;
	}
	AMagneticShield* magneticShield = Cast<AMagneticShield>(OtherActor);
	if (magneticShield)
	{
		SetCanDoDamage(true);
	}
}
