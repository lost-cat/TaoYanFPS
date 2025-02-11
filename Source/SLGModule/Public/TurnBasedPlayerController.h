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
	UFUNCTION(BlueprintCallable)
	APawn* GetSelectedPawn() const;
	AActor* GetHoverActor() const;
	void SetSelectedPawn(ATurnBasedCharactor* Charactor);
	bool GetCursorLocation(FVector& OutLocation) const;

	FORCEINLINE TObjectPtr<UCharacterActionContentWidget> GetCharacterActionContentWidget() const
	{
		return CharacterActionContentWidget;
	}
	
	void BindInputMapping(const TSoftObjectPtr<UInputMappingContext>& InputMappingContext, int32 Priority);
	void RemoveInputMapping(const TSoftObjectPtr<UInputMappingContext>& InputMappingContext);
	UFUNCTION()
	void OnTurnForwarded(const FTurn& NextTurn);

protected:
	virtual void BeginPlay() override;
	void UnSelect();
	void SelectPawn(const FInputActionValue& InputActionValue);
	void OnTargetingCompleted(const FInputActionValue& InputActionValue);
	void CancelTargeting();
	virtual void SetupInputComponent() override;

private:
	

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
	TSoftObjectPtr<UInputMappingContext> TargetingInputMappingContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=GamePlay)
	TSoftObjectPtr<UInputAction> SelectPawnAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> TargetingAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> CancelTargetingAction;

private:
	TObjectPtr<ATurnBasedCharacterBase> SelectedPawn;
	
	TObjectPtr<UCharacterActionContentWidget> CharacterActionContentWidget;
	TObjectPtr<USLGMainWidget>MainWidget;
};
