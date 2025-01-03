// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedEnemy.h"

void ATurnBasedEnemy::OnSelected(APlayerController* PlayerController)
{
	Super::OnSelected(PlayerController);
}

void ATurnBasedEnemy::OnUnSelected(APlayerController* PlayerController)
{
	Super::OnUnSelected(PlayerController);
}

void ATurnBasedEnemy::Attack(ATurnBasedCharacterBase* Target)
{
	Super::Attack(Target);
}

void ATurnBasedEnemy::OnAttacked(ATurnBasedCharacterBase* Attacker)
{
	Super::OnAttacked(Attacker);
}
