// Fill out your copyright notice in the Description page of Project Settings.


#include "TaoYanGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/TransitionUI.h"

UTaoYanGameInstance::UTaoYanGameInstance()
{
	// find the class  of the login widget
	
}

void UTaoYanGameInstance::OpenLevel( const TSoftObjectPtr<UWorld> Level)
{
	TransitionIn();
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this,Level]
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.1f, false);
	
}

void UTaoYanGameInstance::Init()
{
	Super::Init();
	// CreateWidget()
	// TransitionUI = CreateWidget<UTransitionUI>(this,UTransitionUI::StaticClass(), TEXT("UITransition"));
}

void UTaoYanGameInstance::TransitionIn()
{
	if (TransitionUI)
	{
		if ( !TransitionUI->IsInViewport())
		{
			TransitionUI->AddToViewport();
		}
		
		TransitionUI->TransitionIn();
	}
}

void UTaoYanGameInstance::TransitionOut()
{
	if (TransitionUI)
	{
		if ( !TransitionUI->IsInViewport())
		{
			TransitionUI->AddToViewport();
		}
		
		TransitionUI->TransitionOut();
	}
}
