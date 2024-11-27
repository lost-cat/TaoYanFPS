// Copyright Epic Games, Inc. All Rights Reserved.


#include "TaoYanPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerState.h"

void ATaoYanPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ATaoYanPlayerController::GainScore(const float Score) const
{
	auto State = GetPlayerState<APlayerState>();
	State->SetScore(State->GetScore() + Score);
}

float ATaoYanPlayerController::GetScore() const
{
	auto State = GetPlayerState<APlayerState>();
	return State->GetScore();
}
