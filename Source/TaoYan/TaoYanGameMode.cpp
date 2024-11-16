// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaoYanGameMode.h"
#include "TaoYanCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATaoYanGameMode::ATaoYanGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
