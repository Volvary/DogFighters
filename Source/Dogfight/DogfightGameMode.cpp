// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DogfightGameMode.h"
#include "DogfightPawn.h"

ADogfightGameMode::ADogfightGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ADogfightPawn::StaticClass();
}
