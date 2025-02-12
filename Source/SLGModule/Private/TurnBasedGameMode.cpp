// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedGameMode.h"

#include "TurnBasedEnemy.h"
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

void ATurnBasedGameMode::ForwardTurn(ETurnType NextTurnType)
{
	if (TurnRecords.Num() == 0)
	{
		FTurn FirstTurn;
		FirstTurn.TurnIndex = 1;
		FirstTurn.TurnType = ETurnType::PlayerTurn;
		TurnRecords.Add(FirstTurn);
		OnTurnForwarded.Broadcast(FirstTurn);
	}
	else
	{
		const auto [TurnIndex, TurnType] = TurnRecords.Last();
		const FTurn NextTurn{TurnIndex + 1, NextTurnType};
		TurnRecords.Add(NextTurn);
		OnTurnForwarded.Broadcast(NextTurn);
	}
	if (NextTurnType == ETurnType::PlayerTurn)
	{
		ResetAllControlledPawnStates();
	}
	else if (NextTurnType == ETurnType::EnemyTurn)
	{
		ResetAllEnemyPawnStates();
	}
}

void ATurnBasedGameMode::BeginPlay()
{
	Super::BeginPlay();

	const auto PlayerController = Cast<ATurnBasedPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	OnTurnForwarded.AddUniqueDynamic(PlayerController, &ATurnBasedPlayerController::OnTurnForwarded);

	ATurnBasedCharacterBase* PlayerCharacter0 = SpawnCharacterAtLocation(PlayerCharacterClass, FVector(0, 430, 88));
	ATurnBasedCharacterBase* PlayerCharacter1 = SpawnCharacterAtLocation(PlayerCharacterClass, FVector(100, 230, 88));
	ControlledPawns.Add(PlayerCharacter0);
	ControlledPawns.Add(PlayerCharacter1);
	ATurnBasedCharacterBase* Enemy1 = SpawnCharacterAtLocation(EnemyCharacterClass, FVector(0, -230, 88));
	ATurnBasedCharacterBase* Enemy2 = SpawnCharacterAtLocation(EnemyCharacterClass, FVector(0, -100, 88));
	EnemyPawns.Add(Enemy1);
	EnemyPawns.Add(Enemy2);

	ForwardTurn();
}


void ATurnBasedGameMode::ResetAllControlledPawnStates()
{
	for (ATurnBasedCharacterBase* OurPawn : ControlledPawns)
	{
		OurPawn->ResetTurnRelatedState();
	}
}

void ATurnBasedGameMode::ResetAllEnemyPawnStates()
{
	for (ATurnBasedCharacterBase* EnemyPawn : EnemyPawns)
	{
		EnemyPawn->ResetTurnRelatedState();
	}
}

ATurnBasedCharacterBase* ATurnBasedGameMode::GetNextFocusedEnemy()
{
	ATurnBasedCharacterBase** FindByPredicate = EnemyPawns.FindByPredicate([](ATurnBasedCharacterBase* EnemyPawn)
	{
		return EnemyPawn->IsActionable();
	});
	return FindByPredicate ? *FindByPredicate : nullptr;
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
		ATurnBasedCharacterBase* SpawnActor = World->SpawnActor<ATurnBasedCharacterBase>(
			CharacterClass, Location, FRotator::ZeroRotator, SpawnParams);

		return SpawnActor;
	}
	return nullptr;
}
