// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedGameMode.h"

FTurn ATurnBasedGameMode::GetCurrentTurn() const
{
	return TurnRecords.Last();
}

void ATurnBasedGameMode::ForwardTurn(ETurnType NextTurnType = ETurnType::PlayerTurn)
{
	if (TurnRecords.Num() == 0)
	{
		FTurn FirstTurn;
		FirstTurn.TurnIndex = 0;
		FirstTurn.TurnType = ETurnType::PlayerTurn;
		TurnRecords.Add(FirstTurn);
	}
	else
	{
		const auto [TurnIndex, TurnType] = TurnRecords.Last();
		FTurn NextTurn{TurnIndex + 1, NextTurnType};
		TurnRecords.Emplace(NextTurn);
		
	}

}
