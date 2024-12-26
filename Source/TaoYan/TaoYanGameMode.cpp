// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaoYanGameMode.h"
#include "TaoYanPlayerState.h"
#include "TaoYanScalableComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

class ATaoYanPlayerController;


TArray<int> GetRandomUniqueNumbers(const int N, const int S)
{
	TArray<int> UniqueNumbers;
	TSet<int> SelectedNumbers;

	while (UniqueNumbers.Num() < S)
	{
		if (int RandomNumber = FMath::RandRange(0, N - 1); !SelectedNumbers.Contains(RandomNumber))
		{
			SelectedNumbers.Add(RandomNumber);
			UniqueNumbers.Add(RandomNumber);
		}
	}

	return UniqueNumbers;
}


ATaoYanGameMode::ATaoYanGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	static ConstructorHelpers::FClassFinder<AStaticMeshActor> ScoreCubeClassFinder(
		TEXT(
			"/Script/Engine.Blueprint'/Game/FirstPerson/Blueprints/SM_ScalableChamferCube.SM_ScalableChamferCube_C'"));
	ScoreCubeClass = ScoreCubeClassFinder.Class;
}

/**
 * Starts the score timer and sets the initial remaining time.
 * 
 * This function initializes the remaining time for the game state and sets a timer to call the UpdateTime function every second.
 */
void ATaoYanGameMode::StartScoreTime()
{
	GetGameState<ATaoYanGameState>()->SetRemainTime(TimeDuration);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATaoYanGameMode::UpdateTime, 1.0f, true);
}

/**
 * Updates the remaining time for the game state.
 *
 * This function decreases the remaining time by the timer rate and checks if the time has run out.
 * If the remaining time is less than or equal to zero, it clears the timer and logs the game over message.
 * It also logs the score of each player.
 */
void ATaoYanGameMode::UpdateTime()
{
	const auto TimeStep =GetWorld()->GetTimerManager().GetTimerRate(TimerHandle);
	
	GetGameState<ATaoYanGameState>()->SetRemainTime(
		GetGameState<ATaoYanGameState>()->GetRemainTime() -TimeStep
		);
	if (GetGameState<ATaoYanGameState>()->GetRemainTime() <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		// Score Time Over
		UE_LOG(LogTemp, Warning, TEXT("Game Over"));
		for (TObjectPtr PlayerState : GetWorld()->GetGameState()->PlayerArray)
		{
			if (const auto TaoYanPlayerState = Cast<ATaoYanPlayerState>(PlayerState))
			{
				UE_LOG(LogTemp, Warning, TEXT("Player %s Score: %f"), *TaoYanPlayerState->GetPawn()->GetName(),
				       TaoYanPlayerState->GetScore());
			}
		}
	}
}

void ATaoYanGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	StartScoreTime();
	RandomMarkSpecialScoreCube(5);
	if (ScoreCubeClass == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ScoreCubeClass is not set!"));
	}
}


/**
 * Marks a specified number of score cubes as special.
 *
 * This function finds all actors of the ScoreCubeClass and randomly selects a specified number of them
 * to mark as special by setting their ScalableComponent's special property to true.
 *
 * @param Special The number of score cubes to mark as special.
 */
void ATaoYanGameMode::RandomMarkSpecialScoreCube(int Special)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ScoreCubeClass, FoundActors);
	if (FoundActors.Num() > 0)
	{
		const auto Indices = GetRandomUniqueNumbers(FoundActors.Num(), Special);
		for (int i = 0; i < Indices.Num() && i < FoundActors.Num(); ++i)
		{
			const int Index = Indices[i];
			if (const auto ScalableComponent = FoundActors[Index]->GetComponentByClass<UTaoYanScalableComponent>())
			{
				ScalableComponent->SetSpecial(true);
			}
		}
	}
}
