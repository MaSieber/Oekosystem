// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MagneticBox.h"
#include "WorldDiscoveryPlayerController.h"


// Sets default values
AMagneticBox::AMagneticBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	MagneticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryMesh"));
	MagneticMesh->AttachTo(BaseComponent);
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
		AWorldDiscoveryPlayerController* playerController =  Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
		if (playerController)
		{
			float MouseX = 0.0f;
			float MouseY = 0.0f;
			FVector mouseLocation, mouseDirection;
			//bool bMouse = playerController->GetMousePosition(MouseX,MouseY);
			bool bMouse = playerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);

			

			if (bMouse)
			{
				FVector newLocation;
				newLocation.X = GetActorLocation().X;
				newLocation.Y = mouseLocation.Y + (mouseDirection.Y * 500);
				newLocation.Z = mouseLocation.Z + (mouseDirection.Z * 500);

				UE_LOG(LogTemp, Warning, TEXT("actor: %f %f %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
				UE_LOG(LogTemp, Warning, TEXT("mouse: %f %f %f"), mouseLocation.X, mouseLocation.Y,mouseLocation.Z);
				UE_LOG(LogTemp, Warning, TEXT("mouse direction: %f %f %f"), mouseDirection.X, mouseDirection.Y, mouseDirection.Z);
				SetActorLocation(newLocation);
			}
		}
		//if (CurrentForceSeconds > 0.0f)
		//	CurrentForceSeconds -= DeltaTime;
		//else
		//	bPulling = false;

		//MagneticMesh->AddTorque(ForceDirection);
		//MagneticMesh->AddForce(ForceDirection * ForceAmount, NAME_None, true);
		//MagneticMesh->AddImpulse(ForceDirection * ForceAmount, NAME_None, true);
	}

}

void AMagneticBox::triggerMagnetic(FVector direction, float force)
{
	MagneticMesh->SetSimulatePhysics(false);
	bPulling = true;
	ForceDirection = direction;
	ForceAmount = force;
	CurrentForceSeconds = ForceSeconds;
}

void AMagneticBox::TriggerMagneticStop()
{
	bPulling = false;
	MagneticMesh->SetSimulatePhysics(true);

	
}
