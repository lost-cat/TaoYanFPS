// Fill out your copyright notice in the Description page of Project Settings.


#include "TaoYanGameState.h"

#include "Net/UnrealNetwork.h"

void ATaoYanGameState::SetRemainTime(const float InTime)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		RemainTime = InTime;
		OnRemainTimeUpdate();
	}

}

void ATaoYanGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATaoYanGameState,RemainTime);
}

void ATaoYanGameState::OnRep_RemainTime()
{
	OnRemainTimeUpdate();
}

void ATaoYanGameState::OnRemainTimeUpdate()
{
	OnRemainTimeUpdated.Broadcast(RemainTime);
}
