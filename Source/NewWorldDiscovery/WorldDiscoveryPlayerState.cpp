// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "WorldDiscoveryPlayerState.h"
#include "NewWorldDiscoveryGameMode.h"


AWorldDiscoveryPlayerState::AWorldDiscoveryPlayerState()
{
	CurrentEnergy = 0;
}


int32 AWorldDiscoveryPlayerState::GetCurrentEnergy()
{
	return this->CurrentEnergy;
}

void AWorldDiscoveryPlayerState::AddEnergy(int32 energy)
{
	ANewWorldDiscoveryGameMode* gameMode = Cast<ANewWorldDiscoveryGameMode>(GetWorld()->GetAuthGameMode());

	if (gameMode)
	{
		int32 limit = gameMode->GetEnergyLimit();
		if (limit == 0)
			limit = 100;

		CurrentEnergy = FMath::Clamp(CurrentEnergy + energy,0,limit);
	}
	
}

void AWorldDiscoveryPlayerState::RemoveEnergy(int32 energy)
{
	ANewWorldDiscoveryGameMode* gameMode = Cast<ANewWorldDiscoveryGameMode>(GetWorld()->GetAuthGameMode());

	if (gameMode)
	{
		int32 limit = gameMode->GetEnergyLimit();
		if (limit == 0)
			limit = 100;

		CurrentEnergy = FMath::Clamp(CurrentEnergy - energy, 0, limit);
	}
}
