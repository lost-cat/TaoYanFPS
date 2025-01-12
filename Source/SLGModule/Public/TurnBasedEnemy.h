// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedCharacterBase.h"
#include "TurnBasedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SLGMODULE_API ATurnBasedEnemy : public ATurnBasedCharacterBase
{
public:
	virtual void OnSelected(APlayerController* PlayerController) override;
	virtual void OnUnSelected(APlayerController* PlayerController) override;
	virtual void Attack(ATurnBasedCharacterBase* Target) override;
	virtual void OnAttacked(ATurnBasedCharacterBase* Attacker) override;

	void SetTarget(ATurnBasedCharacterBase* InTarget);
	ATurnBasedCharacterBase* GetCurrentTarget() const;

	bool FindSuitableTarget();

private:
	ATurnBasedCharacterBase* CurrentTarget;
	GENERATED_BODY()
};
