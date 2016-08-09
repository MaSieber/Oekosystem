// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MovingPlatform.h"


#include "../WorldDiscoveryPlayerController.h"
#include "../NewWorldDiscoveryCharacter.h"
#include "MagneticBox/MagneticBox.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(true);

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
	RootComponent = BaseComponent;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->bGenerateOverlapEvents = false;
	PlatformMesh->AttachTo(BaseComponent);

	PlatformEnergySocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformEnergySocketMesh"));
	PlatformEnergySocketMesh->bGenerateOverlapEvents = false;
	PlatformEnergySocketMesh->AttachTo(PlatformMesh);

	PlatformEnergySocketMeshRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformEnergySocketMesh2"));
	PlatformEnergySocketMeshRight->bGenerateOverlapEvents = false;
	PlatformEnergySocketMeshRight->AttachTo(PlatformMesh);

	BoxCollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionTrigger"));
	BoxCollisionTrigger->bGenerateOverlapEvents = true;
	BoxCollisionTrigger->SetCollisionProfileName("PlatformTrigger");
	BoxCollisionTrigger->AttachTo(PlatformEnergySocketMesh);

	BoxCollisionTrigger2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionTrigger2"));
	BoxCollisionTrigger2->bGenerateOverlapEvents = true;
	BoxCollisionTrigger2->SetCollisionProfileName("PlatformTrigger");
	BoxCollisionTrigger2->AttachTo(PlatformEnergySocketMeshRight);

	PlatformLineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformLineMesh"));
	PlatformLineMesh->bGenerateOverlapEvents = false;
	PlatformLineMesh->AttachTo(BaseComponent);

	Acceleration = 2.0f;
	MaxPlatformVelocity = 10.0f;
	CurrentVelocity = 0.0f;
	bFixedSpeed = true;

	Type = eTypeDirection::HORIZONTAL;
	InitialDirection = eInitialDirection::END;

	MoveDirection = 1.0f;

	actor = nullptr;
	MagneticObject = nullptr;
	CollisionActor = nullptr;
	ColliderType = 0;

	bStarted = false;

	bSnapDelay = false;
	SnapDelay = 0;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if ((Type == eTypeDirection::HORIZONTAL && InitialDirection == eInitialDirection::END) || (Type == eTypeDirection::VERTICAL && InitialDirection == eInitialDirection::END))
		MoveDirection = -1.0f;
	else
		MoveDirection = 1.0f;

	StartDirection = MoveDirection;

	UE_LOG(LogTemp, Warning, TEXT("Begin Play Platform %f %f"), StartDirection, MoveDirection);

	RelOriginPosition = PlatformMesh->RelativeLocation;
	OriginDirection = InitialDirection;
	bOriginActive = bActive;

	if (objectMagnet != nullptr)
	{
		objectMagnet->SetActorLocation(PlatformEnergySocketMesh->GetComponentLocation());
	}
	if (objectMagnet2 != nullptr)
	{
		objectMagnet2->SetActorLocation(PlatformEnergySocketMeshRight->GetComponentLocation());
	}

	
}

void AMovingPlatform::ResetPlatform(bool bInstant)
{
	if (!bStarted)
		return;

	CurrentVelocity = 0.0f;

	if (bRestAnim == true)
		return;

	if (bInstant)
	{
		PlatformMesh->RelativeLocation = RelOriginPosition;
		bActive = bOriginActive;
		InitialDirection = OriginDirection;
	}
	else
	{
		float dist = FVector::Dist(RelOriginPosition,PlatformMesh->RelativeLocation);
		UE_LOG(LogTemp, Warning, TEXT("Dist %f"), dist);
		if (dist >= 2.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Start Platform %f %f"), StartDirection, MoveDirection);

			if (StartDirection == 1.0f)
				MoveDirection = -1.0f;
			if (StartDirection == -1.0f)
				MoveDirection = 1.0f;

			UE_LOG(LogTemp, Warning, TEXT("Start Platform %f %f"), StartDirection, MoveDirection);
		}
		bRestAnim = true;
		bActive = true;
	}	
}

//ToDo: Write generell Acceleration/Movement-Helper Class
void AMovingPlatform::Accelerate(float DeltaTime)
{
	CurrentVelocity += DeltaTime * Acceleration * MaxPlatformVelocity;
	CurrentVelocity = FMath::Clamp(CurrentVelocity, 0.0f, MaxPlatformVelocity);
}

void AMovingPlatform::SpawnChildActor(AActor* box)
{
	AMagneticBox *energyBox = Cast<AMagneticBox>(CollisionActor);
	if (energyBox && MagneticObject == nullptr)
	{
		FString name = "TempMagneticBox_";
		FMath::RandInit(FMath::Rand());
		name.AppendInt(FMath::RandRange(100, 999));
		FMath::RandInit(FMath::Rand());
		name.AppendInt(FMath::RandRange(100, 999));

		FName ObjectName = FName(*name);
		UChildActorComponent * NewComp = ConstructObject<UChildActorComponent >(UChildActorComponent::StaticClass(), this, ObjectName);
		if (NewComp)
		{
			FVector originScale = energyBox->GetActorScale3D();
			NewComp->RegisterComponent();
			UE_LOG(LogTemp, Warning, TEXT("Registers"));
			NewComp->SetChildActorClass(ObjectToSnap);
			UE_LOG(LogTemp, Warning, TEXT("SetChildActor"));
			NewComp->SetRelativeScale3D(originScale);
			if (ColliderType == 1)
			{
				NewComp->AttachTo(PlatformEnergySocketMesh, NAME_None, EAttachLocation::SnapToTarget);
			}
			else if (ColliderType == 2)
			{
				NewComp->AttachTo(PlatformEnergySocketMeshRight, NAME_None, EAttachLocation::SnapToTarget);
			}
			UE_LOG(LogTemp, Warning, TEXT("Attach"));

			float offsetZ = 125.0f;
			if (Type == eTypeDirection::HORIZONTAL)
				offsetZ = 112.0f;

			NewComp->SetRelativeLocation(FVector(0.0f, 0.0f, offsetZ));
			UE_LOG(LogTemp, Warning, TEXT("Location"));


			MagneticObject = NewComp;
		}
	}
}

// Called every frame
void AMovingPlatform::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bSnapDelay)
	{
		if (SnapDelay <= 0)
		{
			EnablePlatform();
			bSnapDelay = false;
		}
		else
			SnapDelay--;
		return;
	}

	if (bActive)
	{
		if (bFixedSpeed)
		{
			Accelerate(DeltaTime);

			FTransform LineTransform = PlatformLineMesh->GetRelativeTransform();
			FVector LineLocationVector = LineTransform.GetLocation();

			FVector min;
			FVector max;
			PlatformLineMesh->GetLocalBounds(min,max);

			FTransform PlatformTransform = PlatformMesh->GetRelativeTransform();
			FVector PlatformLocationVector = PlatformTransform.GetLocation();

			FVector End = LineLocationVector;
			FVector Start = FVector::ZeroVector;

			if (Type == eTypeDirection::HORIZONTAL)
			{
				Start.Y = End.Y + ((max.Z + min.Z) * LineTransform.GetScale3D().Z);
				PlatformLocationVector.Y += MoveDirection * CurrentVelocity * DeltaTime;

				DirectionSwitch(PlatformLocationVector.Y, Start.Y, End.Y);

			}
			else if (Type == eTypeDirection::VERTICAL)
			{
				Start.Z = End.Z - ((max.Z + min.Z) * LineTransform.GetScale3D().Z);
				PlatformLocationVector.Z += MoveDirection * CurrentVelocity * DeltaTime;

				DirectionSwitch(PlatformLocationVector.Z, End.Z, Start.Z);

			}

			PlatformMesh->SetRelativeLocation(PlatformLocationVector);
		
			if (actor != nullptr)
			{
				if (actor->IsActorBeingDestroyed())
				{
					actor = nullptr;
					return;
				}
				FVector RelLoc = PlatformEnergySocketMesh->RelativeLocation;
				FVector ActorLocation = GetActorLocation() + PlatformLocationVector;

				ActorLocation.Y -= RelLoc.X;
				ActorLocation.Z += RelLoc.Z;
				actor->SetActorLocation(ActorLocation);
			}		

			if (!bStatic && objectMagnet != nullptr)
			{
				objectMagnet->SetActorLocation(PlatformEnergySocketMesh->GetComponentLocation());
			}
			if (!bStatic && objectMagnet2 != nullptr)
			{
				objectMagnet2->SetActorLocation(PlatformEnergySocketMeshRight->GetComponentLocation());
			}

		}
		else
		{
			//Todo: Energy Based... more energy more Speed or acceleration?
		}
	}
}

void AMovingPlatform::DirectionSwitch(float current,float start, float end)
{
	float oldDirection = MoveDirection;

	if (current >= start)
		MoveDirection = -1.0f;
	else if (current <= end)
		MoveDirection = 1.0f;

	if (oldDirection != MoveDirection)
	{
		CurrentVelocity = 0.0f;
		if (bRestAnim)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stop Platform"));
			bRestAnim = false;
			bActive = false;
			return;
		}
	}
		
}

void AMovingPlatform::EnablePlatform()
{
	if (CollisionActor != nullptr && MagneticObject == nullptr)
	{
		AMagneticBox *energyBox = Cast<AMagneticBox>(CollisionActor);
		if (energyBox)
		{
			energyBox->TriggerMagneticStop();
			energyBox->MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
			energyBox->SetNewMassScale(20.0f);
			energyBox->bIgnoreMagnetic = true;

			if (IsDestroyed && !energyBox->IsDestroyed())
			{

				SpawnChildActor(energyBox);

				AWorldDiscoveryPlayerController* playerController = Cast<AWorldDiscoveryPlayerController>(GetWorld()->GetFirstPlayerController());
				if (playerController)
				{
					ANewWorldDiscoveryCharacter *playerChar = Cast<ANewWorldDiscoveryCharacter>(playerController->GetCharacter());
					if (playerChar)
					{
						playerChar->SetCurrentObjectHolder((ABaseObstacle*)this);
					}
				}
				IsDestroyed = false;
				energyBox->TriggerDestroy(true);
			}
			TriggerPlatform(true);
		}
		CollisionActor = nullptr;
		ColliderType = 0;
	}
}

void AMovingPlatform::TriggerPlatform(bool bActiveState)
{
	OnTriggerPlatform(bActiveState);
	this->bActive = bActiveState;
	bStarted = true;
}

void AMovingPlatform::SetStoringEnergy(uint32 energy)
{
	this->StoringEnergy = energy;
}

void AMovingPlatform::OverlapBegin(int32 Collider, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping"));

	AMagneticBox *energyBox = Cast<AMagneticBox>(OtherActor);
	if (energyBox && MagneticObject == nullptr)
	{
		if (!energyBox->bForceShit)
		{ 
			UE_LOG(LogTemp, Warning, TEXT("Overlapping2 %s"), *energyBox->MagneticMesh->GetCollisionProfileName().ToString());
			bSnapDelay = true;
			SnapDelay = 10;
			CollisionActor = OtherActor;
			ColliderType = Collider;
		}
	}
}

void AMovingPlatform::OverlapEnd(int32 Collider, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMagneticBox *energyBox = Cast<AMagneticBox>(OtherActor);
	if (energyBox)
	{
		energyBox->MagneticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Overlap);
		energyBox->bIgnoreMagnetic = false;
		energyBox->MagneticMesh->SetSimulatePhysics(true);
		energyBox->SetNewMassScale(1.0f);
		TriggerPlatform(false);
		actor = nullptr;
	}
}


void AMovingPlatform::DestroyMagneticObject()
{
	TriggerPlatform(false);

	Super::DestroyMagneticObject();
}