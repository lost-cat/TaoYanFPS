// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TAOYAN_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ScoreLabel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* RemainTimeLabel;

	virtual void NativeOnInitialized() override;
private:
	UFUNCTION()
	void UpdateRemainTime(int RemainTime);
	UFUNCTION()
	void UpdateScore(float Score);
};


