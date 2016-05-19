// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "Door.h"

#include "MagneticBox/MagneticBox.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->bGenerateOverlapEvents = true;
	//DoorMesh->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	//DoorMesh->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);
	DoorMesh->AttachTo(BaseComponent);

	DoorSocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorSocketMesh"));
	DoorSocketMesh->bGenerateOverlapEvents = false;
	DoorSocketMesh->AttachTo(BaseComponent);

	bRepeat = true;
	bMoveDoor = false;
	MaxDoorVelocity = 20.0f;
	DoorState = eState::CLOSED;

	RepeatCount = 1;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	FVector RelLocation = DoorMesh->RelativeLocation;
	LineTransform = DoorMesh->GetRelativeTransform();
	DoorMesh->GetLocalBounds(min, max);
	End = RelLocation;
	Start = FVector::ZeroVector;
	if (DoorState == eState::CLOSED)
		Start.Z = End.Z - ((max.Z - min.Z) * LineTransform.GetScale3D().Z);
	else if (DoorState == eState::OPEN)
	{
		Start.Z = End.Z + ((max.Z - min.Z) * LineTransform.GetScale3D().Z);
		End.Z = Start.Z;
		Start = RelLocation;
	}	
}

// Called every frame
void ADoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bMoveDoor && (RepeatCount > 0 || bRepeat))
	{ 
		FVector RelLocation = DoorMesh->RelativeLocation;
		switch (DoorState)
		{
			case eState::CLOSED:
			{
				RelLocation.Z -= MaxDoorVelocity * DeltaTime;
				if (RelLocation.Z <= Start.Z)
				{
					bMoveDoor = false;
					DoorState = eState::OPEN;
					RepeatCount -= 1;
				}
				break;
			}
			case eState::OPEN:
			{
				RelLocation.Z += MaxDoorVelocity * DeltaTime;
				if (RelLocation.Z >= End.Z)
				{
					bMoveDoor = false;
					DoorState = eState::CLOSED;
					RepeatCount -= 1;
				}
				break;
			}
		}
		DoorMesh->SetRelativeLocation(RelLocation);
	}
}

void ADoor::TriggerDoorMove(bool bMove)
{
	bMoveDoor = bMove;
}

void ADoor::OverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMagneticBox *magneticBox = Cast<AMagneticBox>(OtherActor);
	if (magneticBox)
	{
		TriggerDoorMove(true);
	}
}

void ADoor::OverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMagneticBox *magneticBox = Cast<AMagneticBox>(OtherActor);
	if (magneticBox)
	{
		TriggerDoorMove(false);
	}
}

