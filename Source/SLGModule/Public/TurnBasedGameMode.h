// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TurnBasedGameMode.generated.h"

/**
 * 
 */

enum class ETurnType: uint8
{
	PlayerTurn,
	EnemyTurn,
	AllyTurn
};


USTRUCT()
struct FTurn
{
	GENERATED_BODY()
	int32 TurnIndex;
	ETurnType TurnType;
};

UCLASS()
class SLGMODULE_API ATurnBasedGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	FTurn GetCurrentTurn() const;
	void ForwardTurn(ETurnType NextTurnType);
private:
	TArray<FTurn> TurnRecords;
	
};
