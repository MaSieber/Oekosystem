// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "MagneticMovementComponent.h"


UMagneticMovementComponent::UMagneticMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


void UMagneticMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//~~~~~~~~~~~~~~~~~

	//UE_LOG //comp Init!
}

//Tick Comp
void UMagneticMovementComponent::TickComponent(float DeltaTime,enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//UE_LOG //custom comp is ticking!!!

}