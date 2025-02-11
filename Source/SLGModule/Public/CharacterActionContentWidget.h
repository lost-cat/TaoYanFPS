// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "CommonActivatableWidget.h"
#include "TurnBasedCharacterBase.h"
#include "CharacterActionContentWidget.generated.h"

class UTextBlock;
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
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> CharacHP;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> AttackPower;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> CharacMoveRange;

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> MoveAbility;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> AttackAbility;
	
	UFUNCTION()
	void OnCorrespondCharacterMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	UFUNCTION()
	void OnCharacterSelected(class ATurnBasedCharacterBase* InCharacter);

	UFUNCTION()
	void OnCharacterUnSelected(ATurnBasedCharacterBase* InCharacter);
	void SetUIVisibility();
private:
	UFUNCTION()
	void OnStandbyClicked();
	UFUNCTION()
	void OnMoveClicked();
	UFUNCTION()
	void UpdateHP(float NewHP, float OldHP);
	UFUNCTION()
	void UpdateAttackPower(float NewAttackPower, float OldAttackPower);
	UFUNCTION()
	void UpdateMoveRange(float NewMoveRange, float OldMoveRange);
	

private:
	ATurnBasedCharacterBase* CorrespondCharacter;
};


