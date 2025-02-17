// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn/TurnAction.h"

#include "TurnBasedCharacterBase.h"

// UTurnAction_Move  //////////////////////////////////////////////////////////////
void UTurnAction_Move::Execute()
{
	Super::Execute();
	if (IsValid(Target)) //  move to taget actor
	{
	}
	else // move to action location
	{
		StartLocation = BehaviorOwner->GetActorLocation();
	}
	
}

void UTurnAction_Move::Undo()
{
	Super::Undo();
	BehaviorOwner->SetActorLocation(StartLocation);
}

// UTurnAction_Attack  //////////////////////////////////////////////////////////////
void UTurnAction_Attack::Execute()
{
	Super::Execute();
}

void UTurnAction_Attack::Undo()
{
	Super::Undo();
}

// UTurnAction_Standby  //////////////////////////////////////////////////////////////
void UTurnAction_Standby::Execute()
{
	Super::Execute();
}

void UTurnAction_Standby::Undo()
{
	Super::Undo();
}
