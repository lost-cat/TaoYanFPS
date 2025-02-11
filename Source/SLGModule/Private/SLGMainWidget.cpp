// Fill out your copyright notice in the Description page of Project Settings.


#include "SLGMainWidget.h"

#include "CharacterActionContentWidget.h"
#include "TurnBasedGameMode.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void USLGMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EndTurn->OnClicked.AddUniqueDynamic(this, &USLGMainWidget::OnEndTurnButtonClicked);
	if (ATurnBasedGameMode* TurnBasedGameMode = Cast<ATurnBasedGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		TurnBasedGameMode->OnTurnForwarded.AddUniqueDynamic(this, &USLGMainWidget::UpdateTurns);
		UpdateTurns(TurnBasedGameMode->GetCurrentTurn());
	}
}


void USLGMainWidget::OnEndTurnButtonClicked()
{
	if (ATurnBasedGameMode* TurnBasedGameMode = Cast<ATurnBasedGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		UE_LOG(LogTemp, Log, TEXT("End Turn Button Clicked"));
		TurnBasedGameMode->ForwardTurn(ETurnType::EnemyTurn);
	}
}

void USLGMainWidget::UpdateTurns(const FTurn& NextTurn)
{
	int32 TurnIndex = NextTurn.TurnIndex;
	FText TurnText = FText::FromString(FString::FromInt(TurnIndex));
	Turns->SetText(TurnText);
	FString TurnType = NextTurn.TurnType == ETurnType::PlayerTurn ? "Player" : "Enemy";

	switch (NextTurn.TurnType)
	{
	case ETurnType::PlayerTurn:
		CharacterActions->ActionsList->SetVisibility(ESlateVisibility::Visible);
		EndTurn->SetVisibility(ESlateVisibility::Visible);
		break;
	case ETurnType::EnemyTurn:
		EndTurn->SetVisibility(ESlateVisibility::Hidden);
		break;
	default: checkNoEntry();
	}

	TurnDescription->SetText(FText::FromString(TurnType + " Turn"));
}
