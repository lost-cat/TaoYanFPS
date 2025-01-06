// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedGameMode.h"

#include "TurnBasedPlayerController.h"
#include "Kismet/GameplayStatics.h"

ATurnBasedGameMode::ATurnBasedGameMode()
{
	ConstructorHelpers::FClassFinder<ATurnBasedCharacterBase> PlayerCharacterClassFinder(
		TEXT("/Script/Engine.Blueprint'/Game/SLGContent/Pawn/BP_TurnBasedPawn.BP_TurnBasedPawn_C'"));
	PlayerCharacterClass = PlayerCharacterClassFinder.Class;
	ConstructorHelpers::FClassFinder<ATurnBasedCharacterBase> EnemyCharacterClassFinder(
		TEXT("/Script/Engine.Blueprint'/Game/SLGContent/Pawn/BP_TurnBasedEnemy.BP_TurnBasedEnemy_C'"));
	EnemyCharacterClass = EnemyCharacterClassFinder.Class;
}

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

void ATurnBasedGameMode::BeginPlay()
{
	Super::BeginPlay();
	ATurnBasedCharacterBase* PlayerCharacter0 = SpawnCharacterAtLocation(PlayerCharacterClass, FVector(0, 130, 88));
	ATurnBasedCharacterBase* PlayerCharacter1 = SpawnCharacterAtLocation(PlayerCharacterClass, FVector(0, 130, 88));
	auto PlayerController = Cast<ATurnBasedPlayerController>(UGameplayStatics::GetPlayerController(this,0));
	PlayerController->AppendControlledPawn(PlayerCharacter0);
	PlayerController->AppendControlledPawn(PlayerCharacter1);
}

ATurnBasedCharacterBase* ATurnBasedGameMode::SpawnCharacterAtLocation(
	const TSubclassOf<ATurnBasedCharacterBase>& CharacterClass,
	const FVector& Location)
{
	if (CharacterClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterClass is nullptr"));
		return nullptr;
	}
	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ATurnBasedCharacterBase* SpawnActor = World->SpawnActor<ATurnBasedCharacterBase>(CharacterClass, Location, FRotator::ZeroRotator, SpawnParams);
		return SpawnActor;
	}
	return nullptr;
}