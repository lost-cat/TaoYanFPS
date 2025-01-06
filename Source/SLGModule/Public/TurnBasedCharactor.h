// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedCharacterBase.h"
#include "TurnBasedCharactor.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
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

	TObjectPtr<UNiagaraComponent> PathIndicatorComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ShowMovementRange();
	UFUNCTION()
	void ShowOperationContents();

	UFUNCTION()
	void MoveToLocation(const FVector& TargetLocation);
	virtual void Attack(ATurnBasedCharacterBase* Target) override;

	virtual void OnSelected(APlayerController* PlayerController) override;
	virtual void OnUnSelected(APlayerController* PlayerController) override;

private:
	void UpdatePathIndicator();
	void ShowNiagaraPath();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Effects", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UNiagaraSystem> PathIndicator;

	uint32 PawnMoveInputActionHandle = -1;
	uint32 AttackInputActionHandle = -1;
	FTimerHandle UpdatePathTimerHandle;
};
