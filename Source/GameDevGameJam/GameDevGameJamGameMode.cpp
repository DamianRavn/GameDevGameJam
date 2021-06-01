// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameDevGameJamGameMode.h"
#include "GameDevGameJamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameDevGameJamGameMode::AGameDevGameJamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
