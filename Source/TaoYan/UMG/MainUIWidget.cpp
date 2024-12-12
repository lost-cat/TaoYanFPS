// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TaoYan/TaoYanGameMode.h"
#include "TaoYan/TaoYanPlayerController.h"
#include "TaoYan/TP_WeaponComponent.h"

void UMainUIWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// bind remain time update
	if (auto GameMode = GetWorld()->GetAuthGameMode(); GameMode)
	{
		auto TaoYanGameMode = Cast<ATaoYanGameMode>(GameMode);
		TaoYanGameMode->OnRemainTimeUpdate.AddUFunction(this, "UpdateRemainTime");
	}

	// bind score update
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController<ATaoYanPlayerController>())
	{
		PlayerController->OnScoreUpdate.AddUFunction(this, "UpdateScore");
	}

	// hind front sight because pawn default don't have weapon
	FrontSightImage->SetVisibility(ESlateVisibility::Hidden);
	InitialFrontSightPosition = FrontSightImage->GetRenderTransform().Translation;
}

void UMainUIWidget::UpdateRemainTime(const int RemainTime)
{
	RemainTimeLabel->SetText(FText::FromString(FString::Printf(TEXT("%d"), RemainTime)));
}

void UMainUIWidget::UpdateScore(const float Score)
{
	ScoreLabel->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Score)));
}


void UMainUIWidget::MakeFrontSightFollowsRecoil(const float Recoil)
{
	if (Recoil == 0)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("update front sight"));

	FVector2D NewPosition = InitialFrontSightPosition - FVector2D(0, Recoil * 10); // 调整乘数以控制移动速度
	FrontSightImage->SetRenderTranslation(NewPosition);
}