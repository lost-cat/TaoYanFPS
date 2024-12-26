// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TransitionUI.generated.h"

/**
 * 
 */
UCLASS()
class TAOYAN_API UTransitionUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void TransitionIn();
	void TransitionOut();
public:
	UPROPERTY(BlueprintReadOnly,Transient,meta=(BindWidgetAnim))
	UWidgetAnimation* TransitionAnimation;
};
