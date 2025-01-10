// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "CommonActivatableWidget.h"
#include "TurnBasedCharacterBase.h"
#include "CharacterActionContentWidget.generated.h"

class USizeBox;
class UButton;
/**
 * 
 */
UCLASS()
class SLGMODULE_API UCharacterActionContentWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()



	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Standby;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Move;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Attack;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> OpenItem;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<USizeBox> ActionsList;

public:
	UFUNCTION()
	void OnCorrespondCharacterMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	UFUNCTION()
	void SetCorrespondCharacter(class ATurnBasedCharacterBase* InCharacter);
	void UpdateUI();
	UFUNCTION()
	void OnPlayerUnSelected(ATurnBasedCharacterBase* InCharacter);
private:
	UFUNCTION()
	void OnStandbyClicked();
	UFUNCTION()
	void OnMoveClicked();
	

private:
	ATurnBasedCharacterBase* CorrespondCharacter;
};
