// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedEnemy.h"

#include "TurnBasedGameMode.h"

void ATurnBasedEnemy::OnSelected(APlayerController* PlayerController)
{
	Super::OnSelected(PlayerController);
}

void ATurnBasedEnemy::OnUnSelected(APlayerController* PlayerController)
{
	Super::OnUnSelected(PlayerController);
}

void ATurnBasedEnemy::Attack_Implementation(ATurnBasedCharacterBase* Target)
{
	Super::Attack_Implementation(Target);
}

void ATurnBasedEnemy::OnAttacked(ATurnBasedCharacterBase* Attacker)
{
	Super::OnAttacked(Attacker);
}

void ATurnBasedEnemy::SetTarget(ATurnBasedCharacterBase* InTarget)
{
	CurrentTarget = InTarget;
}

ATurnBasedCharacterBase* ATurnBasedEnemy::GetCurrentTarget() const
{
	return CurrentTarget;
}

bool ATurnBasedEnemy::FindSuitableTarget()
{
	CurrentTarget = nullptr; //  reset target
	ATurnBasedGameMode* GameMode = GetWorld()->GetAuthGameMode<ATurnBasedGameMode>();
	check(GameMode);
	float MinDistance = TNumericLimits<float>::Max();
	// find the nearest player controlled pawn and set it as target
	for (ATurnBasedCharacterBase* PlayerControlledPawn : GameMode->GetPlayerControlledPawns())
	{
		if (PlayerControlledPawn == nullptr)
		{
			continue;
		}
		FVector Location = PlayerControlledPawn->GetActorLocation();
		if (auto Distance = FVector::Dist(Location, GetActorLocation()); Distance < MinDistance)
		{
			MinDistance = Distance;
			CurrentTarget = PlayerControlledPawn;
		}
	}
	return CurrentTarget != nullptr;
}
