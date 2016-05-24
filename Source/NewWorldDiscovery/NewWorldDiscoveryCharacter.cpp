// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "NewWorldDiscovery.h"
#include "NewWorldDiscoveryCharacter.h"

#include "MagneticBox/BaseMagnetic.h"
#include "MagneticBox/MagneticBox.h"
#include "MagneticBox/MagneticEnergyProvider.h"
#include "MagneticBox/MagneticEnergyTransfer.h"

ANewWorldDiscoveryCharacter::ANewWorldDiscoveryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	magneticTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));	
	magneticTrigger->bGenerateOverlapEvents = false;
	magneticTrigger->SetSimulatePhysics(false);
	magneticTrigger->AttachTo(RootComponent);
	
	PulledObject = nullptr;
	MaxBalls = 1;
	MaxBoxes = 1;
	MaxPyramides = 1;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANewWorldDiscoveryCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveRight", this, &ANewWorldDiscoveryCharacter::MoveRight);
	InputComponent->BindAxis("RotateAround", this, &ANewWorldDiscoveryCharacter::RotateAround);


	InputComponent->BindAction("CreateBox", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CreateMagneticBox);
	InputComponent->BindAction("CreatePyramide", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CreateMagneticPyramide);
	InputComponent->BindAction("CreateBall", IE_Pressed, this, &ANewWorldDiscoveryCharacter::CreateMagneticBall);


	InputComponent->BindTouch(IE_Pressed, this, &ANewWorldDiscoveryCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ANewWorldDiscoveryCharacter::TouchStopped);
}

void ANewWorldDiscoveryCharacter::CreateMagneticBox()
{
	if (CreatedBoxes.Num() >= MaxBoxes)
	{
		Cast<AMagneticBox>(CreatedBoxes[0])->TriggerDestroy(true);
		CreatedBoxes.RemoveAt(0);
	}

	float xDir = 1.0f;
	FVector forward = GetActorForwardVector();
	xDir = forward.Y <= 0 ? -1.0f : 1.0f;

	FVector direction = FVector(0.0f,xDir,0.7f);

	FVector SpawnLocation = GetActorLocation() + direction * 100.0f;
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FRotator Rotation = FRotator(0.0f,0.0f,0.0f);
	AActor *box = GetWorld()->SpawnActor<AMagneticBox>(SpawnLocation,Rotation,SpawnParameters);
	CreatedBoxes.Add(box);
}
void ANewWorldDiscoveryCharacter::CreateMagneticBall()
{

}
void ANewWorldDiscoveryCharacter::CreateMagneticPyramide()
{

}

void ANewWorldDiscoveryCharacter::RotateAround(float Value)
{
	if (PulledObject != nullptr)
	{
		ABaseMagnetic* BaseMagnetic = Cast<ABaseMagnetic>(PulledObject);
		if (BaseMagnetic)
		{
			BaseMagnetic->SetRotationRate(Value);
		}
	}
}

void ANewWorldDiscoveryCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ANewWorldDiscoveryCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ANewWorldDiscoveryCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ANewWorldDiscoveryCharacter::Tick(float DeltaTime)
{

	UCharacterMovementComponent *movementComp = GetCharacterMovement();
	FVector velocity = movementComp->Velocity;
	velocity.X = 0.0f;
	movementComp->Velocity = velocity;

	//To Trigger the Collision Event, if we stay still
	float rad = magneticTrigger->GetUnscaledSphereRadius();
	magneticTrigger->SetSphereRadius(rad + FMath::Sin(DeltaTime)/10.0f);
}

AActor* ANewWorldDiscoveryCharacter::GetPulledObject()
{
	return this->PulledObject;
}