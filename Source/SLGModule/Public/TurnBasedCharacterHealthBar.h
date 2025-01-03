// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnBasedCharacterHealthBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SLGMODULE_API UTurnBasedCharacterHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthBar",meta=(BindWidget))
	UProgressBar* HealthBar;

	UFUNCTION()

	void UpdateHealthBarPercent(float Percent);
	// void UpdateHealthBarMaxLength(float MaxLength);

};


