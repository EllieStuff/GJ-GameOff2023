// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameOff2023GameMode.h"
#include "GameOff2023HUD.h"
#include "GameOff2023Character.h"
#include "UObject/ConstructorHelpers.h"

AGameOff2023GameMode::AGameOff2023GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGameOff2023HUD::StaticClass();
}
