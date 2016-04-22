// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "NewWorldDiscovery.h"
#include "NewWorldDiscoveryGameMode.h"
#include "NewWorldDiscoveryCharacter.h"

ANewWorldDiscoveryGameMode::ANewWorldDiscoveryGameMode()
{
	DefaultPawnClass = ANewWorldDiscoveryCharacter::StaticClass();
}
