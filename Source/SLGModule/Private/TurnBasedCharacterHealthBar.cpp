// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCharacterHealthBar.h"

#include "Components/ProgressBar.h"

inline void UTurnBasedCharacterHealthBar::UpdateHealthBarPercent(float Percent)
{
	HealthBar->SetPercent(Percent);
}
