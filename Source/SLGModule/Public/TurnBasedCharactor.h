// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedCharacterBase.h"
#include "TurnBasedCharactor.generated.h"

class ATurnBasedPlayerController;
class UWidgetComponent;
class UInputAction;

UCLASS()
class SLGMODULE_API ATurnBasedCharactor : public ATurnBasedCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATurnBasedCharactor();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ShowOperationContents();

	UFUNCTION()
	void MoveToLocation(const FVector& TargetLocation);
	virtual void Attack(ATurnBasedCharacterBase* Target) override;

	virtual void OnSelected(APlayerController* PlayerController) override;
	virtual void OnUnSelected(APlayerController* PlayerController) override;

private:
	uint32 PawnMoveInputActionHandle = -1;
	uint32 AttackInputActionHandle = -1;
};
