// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedCharacterBase.h"
#include "GameFramework/GameModeBase.h"
#include "Turn/TurnManagerSubsystem.h"
#include "TurnBasedGameMode.generated.h"

/**
 * 
 */

class ATurnBasedEnemy;
class ATurnBasedCharacterBase;




USTRUCT()
struct FTurn
{
	GENERATED_BODY()
	int32 TurnIndex;
	ETurnType TurnType;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnChanged, const FTurn&, NextTurn);

UCLASS()
class SLGMODULE_API ATurnBasedGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ATurnBasedGameMode();

public:
	FTurn GetCurrentTurn() const;

	void ForwardTurn(ETurnType NextTurnType = ETurnType::PlayerTurn);

	ATurnBasedCharacterBase* SpawnCharacterAtLocation(const TSubclassOf<ATurnBasedCharacterBase>& CharacterClass,
	                                                  const FVector& Location);

	FORCEINLINE TArray<ATurnBasedCharacterBase*> GetPlayerControlledPawns() const { return ControlledPawns; }
	FOnTurnChanged OnTurnForwarded;
	ATurnBasedCharacterBase* GetNextFocusedEnemy();

protected:
	virtual void BeginPlay() override;

private:
	void ResetAllControlledPawnStates();
	void ResetAllEnemyPawnStates();

private:
	TArray<FTurn> TurnRecords;
	UPROPERTY()
	TArray<ATurnBasedCharacterBase*> CharacterActionSequence;

	UPROPERTY()
	TArray<ATurnBasedCharacterBase*> ControlledPawns;

	UPROPERTY()
	TArray<ATurnBasedCharacterBase*> EnemyPawns;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=GamePlay, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ATurnBasedCharacterBase> PlayerCharacterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=GamePlay, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ATurnBasedCharacterBase> EnemyCharacterClass;
};
