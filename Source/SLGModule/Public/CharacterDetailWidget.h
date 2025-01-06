// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CharacterDetailWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SLGMODULE_API UCharacterDetailWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UButton> Standby;
};
