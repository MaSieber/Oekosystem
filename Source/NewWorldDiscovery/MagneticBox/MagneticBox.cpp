// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MagneticBox.h"
#include "NewWorldDiscoveryCharacter.h"
#include "WorldDiscoveryPlayerController.h"
#include "DrawDebugHelpers.h"


// Sets default values
AMagneticBox::AMagneticBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	MagneticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryMesh"));
	MagneticMesh->AttachTo(BaseComponent);
	MagneticMesh->bGenerateOverlapEvents = true;
	MagneticMesh->SetCollisionProfileName("MagneticBox");
	MagneticMesh->OnComponentBeginOverlap.AddDynamic(this, &AMagneticBox::OnOverlapBegin);
	MagneticMesh->OnComponentEndOverlap.AddDynamic(this, &AMagneticBox::OnOverlapEnd);
	MagneticMesh->SetSimulatePhysics(true);
	
	bPulling = false;

	ForceSeconds = 1.0f;
	CurrentForceSeconds = 0.0f;
}

// Called when the game starts or when spawned
void AMagneticBox::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMagneticBox::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bPulling)
	{
		ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		TargetLocation = playerChar->GetActorLocation();
		TargetLocation.Z += 200.0f;

		FVector ActorPos = MagneticMesh->RelativeLocation;

		float dist = FVector::Dist(TargetLocation, ActorPos);
		if (dist > 50)
		{ 
			ForceDirection = (TargetLocation - ActorPos).GetSafeNormal();
			FVector newLocation = ForceAmount * ForceDirection;
		
			DrawDebugLine(GetWorld(), ActorPos, TargetLocation,FColor(255,255,0,1));

			MagneticMesh->AddTorque(ForceDirection);
			MagneticMesh->AddForce(newLocation, NAME_None, true);		
		}
	}

}

void AMagneticBox::triggerMagnetic(FVector direction, float force)
{
	//MagneticMesh->SetSimulatePhysics(false);
	bPulling = true;
	ForceDirection = direction.GetSafeNormal();
	ForceAmount = force;
	CurrentForceSeconds = ForceSeconds;
	
}

void AMagneticBox::TriggerMagneticStop()
{
	bPulling = false;
}

void AMagneticBox::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Overlap Begin"));

	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		FVector playerPos = playerChar->GetActorLocation();
		playerPos.Z += 200.0f;


		FVector direction = playerPos - MagneticMesh->RelativeLocation;
		triggerMagnetic(direction, 1500);
	}
}

void AMagneticBox::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
	TriggerMagneticStop();
}