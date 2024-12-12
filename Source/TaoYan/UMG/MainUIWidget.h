// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIWidget.generated.h"

class UScaleBox;
class UTP_WeaponComponent;
class UImage;
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

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UImage* FrontSightImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScaleBox* BulletScaleBox;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* RemainBulletCountLabel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CurrentBulletCountLabel;
	
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void UpdateWeaponRelatedUI(UTP_WeaponComponent* WeaponComponent);
private:
	UFUNCTION()
	void UpdateRemainTime(int RemainTime);
	UFUNCTION()
	void UpdateScore(float Score);

	UFUNCTION()
	void UpdateFrontSightPosition(float Recoil);
	FVector2D InitialFrontSightPosition;
};


