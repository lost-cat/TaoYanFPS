// Fill out your copyright notice in the Description page of Project Settings.


#include "TaoYanPlayerState.h"

void ATaoYanPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	OnScoreUpdate.Broadcast(GetScore());
}
