// Fill out your copyright notice in the Description page of Project Settings.


#include "TransitionUI.h"

#include "Animation/WidgetAnimation.h"

void UTransitionUI::TransitionIn()
{
	this->PlayAnimation(TransitionAnimation);
}

void UTransitionUI::TransitionOut()
{
	this->PlayAnimation(TransitionAnimation, 0, 1, EUMGSequencePlayMode::Reverse, 1);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]
	{
		this->RemoveFromParent();
	});
	FTimerHandle TimerHandle;
	const float EndTime = TransitionAnimation->GetEndTime();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, EndTime, false);
}
