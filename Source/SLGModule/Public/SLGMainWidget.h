// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SLGMainWidget.generated.h"

struct FTurn;
class UTextBlock;
class UButton;
class UCharacterActionContentWidget;
/**
 * 
 */
UCLASS()
class SLGMODULE_API USLGMainWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UCharacterActionContentWidget> CharacterActions;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UButton> EndTurn;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UTextBlock> Turns;
	UFUNCTION()
	void OnEndTurnButtonClicked();
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void UpdateTurns(const FTurn& NextTurn);

public:
	TObjectPtr<UCharacterActionContentWidget> GetCharacterActionsWidget() const { return CharacterActions; }
};
