// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidget.h"

#include "Components/TextBlock.h"
#include "TaoYan/TaoYanGameMode.h"
#include "TaoYan/TaoYanPlayerController.h"

void UMainUIWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (auto GameMode = GetWorld()->GetAuthGameMode();GameMode) {
		 auto TaoYanGameMode = Cast<ATaoYanGameMode>(GameMode);
		TaoYanGameMode->OnRemainTimeUpdate.AddUFunction(this, "UpdateRemainTime");
	}
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController<ATaoYanPlayerController>())
	{
		PlayerController->OnScoreUpdate.AddUFunction(this, "UpdateScore");
	}
}

void UMainUIWidget::UpdateRemainTime(const int RemainTime)
{
	RemainTimeLabel->SetText(FText::FromString(FString::Printf(TEXT("%d"), RemainTime)));
}
void UMainUIWidget::UpdateScore(const float Score)
{
	ScoreLabel->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Score)));
}
