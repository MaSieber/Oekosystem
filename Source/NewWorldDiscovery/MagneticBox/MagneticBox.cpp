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

	MagneticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryMesh"));
	MagneticMesh->bGenerateOverlapEvents = true;
	MagneticMesh->SetCollisionProfileName("MagneticBox");
	MagneticMesh->OnComponentBeginOverlap.AddDynamic(this, &AMagneticBox::OnOverlapBegin);
	MagneticMesh->OnComponentEndOverlap.AddDynamic(this, &AMagneticBox::OnOverlapEnd);
	MagneticMesh->SetSimulatePhysics(true);
	
	RootComponent = MagneticMesh;

	ForceSeconds = 1.0f;
	CurrentForceSeconds = 0.0f;

	PullingType = ePulling::NONE;

	CurrentVelocity = 0;
	Velocity = 100;
	Acceleration = 20;

	RotationVelocity = 100.0f;

	RotationAmplitude = 1.0f;
	RotationFrequency = 0.02f;

	PushAmount = 5000.0f;
}

// Called when the game starts or when spawned
void AMagneticBox::BeginPlay()
{
	Super::BeginPlay();
}

void AMagneticBox::Accelerate(float DeltaTime)
{
	CurrentVelocity += DeltaTime * Acceleration * Velocity;
	FMath::Clamp(CurrentVelocity,0.0f,Velocity);
}

// Called every frame
void AMagneticBox::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	switch (PullingType)
	{
		case ePulling::NONE:
			break;
		case ePulling::PULLING:
		{
			ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			FVector ActorPos = GetActorLocation();

			TargetLocation = playerChar->GetActorLocation();
			TargetLocation.Z += 200.0f;
			float dist = FVector::Dist(TargetLocation, ActorPos);

			if (dist > 10)
			{
				ForceDirection = (TargetLocation - ActorPos).GetSafeNormal();
				Accelerate(DeltaTime);
				FVector newLocation = ActorPos + CurrentVelocity * ForceDirection * DeltaTime;

				DrawDebugLine(GetWorld(), ActorPos, TargetLocation, FColor(255, 255, 0, 1));

				SetActorLocation(newLocation);

				FRotator rot = GetActorRotation();
				float xDir = 1.0f;
				FVector forward = playerChar->GetActorForwardVector();
				xDir = forward.Y <= 0 ? -1.0f : 1.0f;
				rot.Roll += xDir * 80.0f * FMath::Sin(DeltaTime);
				SetActorRotation(rot);

			}
			else
				PullingType = ePulling::FOLLOWING;

			break;
		}
		case ePulling::FOLLOWING:
		{
			AWorldDiscoveryPlayerController* playerController = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
			ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(playerController->GetCharacter());
			FVector ActorPos = GetActorLocation();

			playerChar->PulledObject = this;

			TargetLocation = playerChar->GetActorLocation();
			TargetLocation.Z += 200.0f;
			
			//Set Location to Player Location
			FVector newLocation = TargetLocation;
			SetActorLocation(TargetLocation);

			//Rotate
			float xDir = 1.0f;
			FVector forward = playerChar->GetActorForwardVector();
			xDir = forward.Y <= 0 ? -1.0f : 1.0f;
			FRotator rot = GetActorRotation();
			rot.Roll += xDir * RotationVelocity * FMath::Sin(DeltaTime);
			SetActorRotation(rot);

			{
				//Rotate AroundMouse
				float MouseY = 0.0f;
				FVector mouseLocation, mouseDirection;
				bool bMouse = playerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
				if (bMouse)
				{
					MouseY = (mouseDirection.Y * 360);
				}

				float s = RotationAmplitude * FMath::Sin(RotationFrequency * MouseY);
				float c = RotationAmplitude * FMath::Cos(RotationFrequency * MouseY);
				
				FVector box = GetActorLocation();
				FVector player = playerChar->GetActorLocation();

				float newY = player.Y + (c * (box.Y - player.Y) - s * (box.Z - player.Z));
				float newZ = player.Z + (s * (box.Y - player.Y) + c * (box.Z - player.Z));
				
				FVector newLocation = FVector(box.X, newY, newZ);
				SetActorLocation(newLocation);
				
			}
			break;

		}
		case ePulling::PUSHING:
		{
			AWorldDiscoveryPlayerController* playerController = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
			ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(playerController->GetCharacter());
			FVector ActorPos = GetActorLocation();
			FVector PlayerPos = playerChar->GetActorLocation();

			FVector PushDirection = -1.0f * (PlayerPos - ActorPos).GetSafeNormal();

			MagneticMesh->AddImpulseAtLocation(PushDirection * PushAmount, GetActorLocation());

			//PullingType = ePulling::NONE;

			break;
		}
	}
}

void AMagneticBox::triggerMagnetic(FVector direction, float force)
{
	if (PullingType == ePulling::NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger"));

		ForceDirection = direction.GetSafeNormal();
		ForceAmount = force;
		CurrentForceSeconds = ForceSeconds;
		MagneticMesh->SetEnableGravity(false);
		MagneticMesh->SetSimulatePhysics(false);
		PullingType = ePulling::PULLING;
		CurrentVelocity = 0;
	}
}

void AMagneticBox::TriggerMagneticStop()
{
	PullingType = ePulling::NONE;
	MagneticMesh->SetEnableGravity(true);
	MagneticMesh->SetSimulatePhysics(true);
}

void AMagneticBox::TriggerMagneticPush()
{
	if (PullingType == ePulling::FOLLOWING)
	{
		UE_LOG(LogTemp, Warning, TEXT("Push"));

		PullingType = ePulling::PUSHING;
		MagneticMesh->SetEnableGravity(true);
		MagneticMesh->SetSimulatePhysics(true);
	}
}

void AMagneticBox::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Overlap Begin"));

	ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(OtherActor);
	if (playerChar)
	{
		USphereComponent* sphereComponent = Cast<USphereComponent>(OtherComp);
		FString name = sphereComponent->GetReadableName();
		UE_LOG(LogTemp, Warning, TEXT("%s"),*name);
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