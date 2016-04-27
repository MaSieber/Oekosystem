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

int32 AWorldDiscoveryPlayerState::GetCurrentRessource()
{
	return this->CurrentRessource;
}

void AWorldDiscoveryPlayerState::AddRessource(int32 ressource)
{
	ANewWorldDiscoveryGameMode* gameMode = Cast<ANewWorldDiscoveryGameMode>(GetWorld()->GetAuthGameMode());

	if (gameMode)
	{
		int32 limit = gameMode->GetRessourceLimit();
		if (limit == 0)
			limit = 100;

		CurrentRessource = FMath::Clamp(CurrentRessource + ressource, 0, limit);
	}

}

void AWorldDiscoveryPlayerState::RemoveRessource(int32 ressource)
{
	ANewWorldDiscoveryGameMode* gameMode = Cast<ANewWorldDiscoveryGameMode>(GetWorld()->GetAuthGameMode());

	if (gameMode)
	{
		int32 limit = gameMode->GetRessourceLimit();
		if (limit == 0)
			limit = 100;

		CurrentRessource = FMath::Clamp(CurrentRessource - ressource, 0, limit);
	}
}
