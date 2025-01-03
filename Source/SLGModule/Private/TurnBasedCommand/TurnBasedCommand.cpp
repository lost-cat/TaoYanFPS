// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCommand.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "TurnBasedCharactor.h"
#include "Runtime/AIModule/Classes/AIController.h"

FTurnBasedCommandMoveto::FTurnBasedCommandMoveto(ATurnBasedCharactor* TargetCharacter, const FVector& TargetLocation):
	Character(TargetCharacter),
	TargetLocation(TargetLocation)
{
	StartLocation = TargetCharacter->GetActorLocation();
}

void FTurnBasedCommandMoveto::Execute()
{
	if (Character)
	{
		// UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(Character->GetWorld(),StartLocation,TargetLocation);
		if (AAIController* Controller = Character->GetController<AAIController>())
		{
			auto RequestResult = Controller->MoveToLocation(TargetLocation);

		}
	}
}
