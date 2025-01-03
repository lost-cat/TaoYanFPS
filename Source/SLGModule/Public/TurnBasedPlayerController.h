// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"

#include "GameFramework/PlayerController.h"
#include "TurnBasedPlayerController.generated.h"

class UInputAction;
class ATurnBasedCharactor;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SLGMODULE_API ATurnBasedPlayerController : public APlayerController
{
	GENERATED_BODY()
	ATurnBasedPlayerController();


public:
	void SetSelectedPawn(ATurnBasedCharactor* Charactor);

protected:
	virtual void BeginPlay() override;
	void UnSelect();
	void SelectPawn(const FInputActionValue& InputActionValue);
	virtual void SetupInputComponent() override;


private:
	void BindInputMapping(const TSoftObjectPtr<UInputMappingContext>& InputMappingContext, int32 Priority);


	
public:
  	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category=GamePlay)
  	TSoftObjectPtr<UInputMappingContext> BaseMovingInputMappingContext;
  
  	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category=GamePlay)
  	TSoftObjectPtr<UInputMappingContext> PawnOperationInputMappingContext;
  
  	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category=GamePlay)
  	TSoftObjectPtr<UInputAction> SelectPawnAction;
  private:
  	TObjectPtr<ATurnBasedCharactor> SelectedPawn;
  };
