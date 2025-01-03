// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "TurnBasedCharacterBase.h"
#include "GameFramework/Character.h"
#include "TurnBasedCharactor.generated.h"

class UWidgetComponent;
class UInputAction;

UCLASS()
class SLGMODULE_API ATurnBasedCharactor : public ATurnBasedCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATurnBasedCharactor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> PawnMoveAction;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveTo();

	UFUNCTION()
	virtual void Attack(ATurnBasedCharacterBase* Target) override;
	virtual void OnSelected(APlayerController* PlayerController) override;
	virtual void OnUnSelected(APlayerController* PlayerController) override;

private:
	uint32 PawnMoveInputActionHandle = -1;
	
};
