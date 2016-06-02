// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "FirePipe.h"

#include "../NewWorldDiscoveryCharacter.h"

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
	CollisionTrigger->AttachTo(FireMesh);

	fireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	fireParticle->bGenerateOverlapEvents = false;
	fireParticle->bAutoActivate = true;
	fireParticle->AttachTo(FireMesh);

	bCanBeDamaged = true;
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

}

void AFirePipe::SetCanDoDamage(bool bDamage)
{
	this->bCanBeDamaged = bDamage;
}

void AFirePipe::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCanBeDamaged)
	{
		ANewWorldDiscoveryCharacter* playerCharacter = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
		if (playerCharacter)
		{
			playerCharacter->Reset();
			UE_LOG(LogTemp,Warning,TEXT("Overlap FirePipe PlayerCharacter"))
		}
	}
}
