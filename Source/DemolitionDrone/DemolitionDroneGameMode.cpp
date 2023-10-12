// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemolitionDroneGameMode.h"
#include "DemolitionDroneCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADemolitionDroneGameMode::ADemolitionDroneGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
