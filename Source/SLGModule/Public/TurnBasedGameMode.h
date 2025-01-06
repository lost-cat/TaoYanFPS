// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedCharacterBase.h"
#include "GameFramework/GameModeBase.h"
#include "TurnBasedGameMode.generated.h"

/**
 * 
 */

class ATurnBasedCharacterBase;

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

	ATurnBasedGameMode();

public:
	FTurn GetCurrentTurn() const;
	void ForwardTurn(ETurnType NextTurnType);

	ATurnBasedCharacterBase* SpawnCharacterAtLocation(const TSubclassOf<ATurnBasedCharacterBase>& CharacterClass,
	                                                  const FVector& Location);

protected:
	virtual void BeginPlay() override;

private:
	TArray<FTurn> TurnRecords;
	UPROPERTY()
	TArray<ATurnBasedCharacterBase*> CharacterActionSequence;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=GamePlay, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ATurnBasedCharacterBase> PlayerCharacterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=GamePlay, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ATurnBasedCharacterBase> EnemyCharacterClass;
};


