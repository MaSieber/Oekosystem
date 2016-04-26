// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "WorldDiscoveryPlayerController.h"


void AWorldDiscoveryPlayerController::BeginPlay()
{
	Super::BeginPlay();


	this->bShowMouseCursor = true;
	this->bEnableClickEvents = true;
	this->bEnableMouseOverEvents = true;
}

