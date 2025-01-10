// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TurnBasedGameMode.h"

#include "GameFramework/PlayerController.h"
#include "TurnBasedPlayerController.generated.h"

class USLGMainWidget;
class UCharacterActionContentWidget;
class ATurnBasedCharacterBase;
class UInputAction;
class ATurnBasedCharactor;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, ATurnBasedCharacterBase*, Character);

/**
 * 
 */
UCLASS()
class SLGMODULE_API ATurnBasedPlayerController : public APlayerController
{
	GENERATED_BODY()
	ATurnBasedPlayerController();

public:
	APawn* GetSelectedPawn() const;
	AActor* GetHoverActor() const;
	void SetSelectedPawn(ATurnBasedCharactor* Charactor);
	bool GetCursorLocation(FVector& OutLocation) const;

	FORCEINLINE TObjectPtr<UCharacterActionContentWidget> GetCharacterActionContentWidget() const
	{
		return CharacterActionContentWidget;
	}

	void AppendControlledPawn(ATurnBasedCharacterBase* PlayerCharacter);
	void BindInputMapping(const TSoftObjectPtr<UInputMappingContext>& InputMappingContext, int32 Priority);
	void RemoveInputMapping(const TSoftObjectPtr<UInputMappingContext>& InputMappingContext);
	UFUNCTION()
	void OnTurnForwarded(const FTurn& NextTurn);

protected:
	virtual void BeginPlay() override;
	void UnSelect();
	void SelectPawn(const FInputActionValue& InputActionValue);
	virtual void SetupInputComponent() override;

private:
	void  ResetAllControlledPawnStates();

public:

	FOnCharacterSelected OnSelectedDelegate;
	FOnCharacterSelected OnUnSelectedDelegate;

	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=GamePlay)
	TSoftObjectPtr<UInputMappingContext> BaseMovingInputMappingContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=UI)
	TSoftClassPtr<UCharacterActionContentWidget> CharacterActionContentWidgetClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=UI)
	TSoftClassPtr<UUserWidget> UIMainClass;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=GamePlay)
	TSoftObjectPtr<UInputMappingContext> PawnOperationInputMappingContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=GamePlay)
	TSoftObjectPtr<UInputAction> SelectPawnAction;

private:
	TObjectPtr<ATurnBasedCharacterBase> SelectedPawn;
	TArray<ATurnBasedCharacterBase*> ControlledPawns;
	TObjectPtr<UCharacterActionContentWidget> CharacterActionContentWidget;
	TObjectPtr<USLGMainWidget>MainWidget;
};
