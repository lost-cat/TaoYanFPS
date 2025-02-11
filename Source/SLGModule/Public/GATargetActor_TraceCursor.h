// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "GATargetActor_TraceCursor.generated.h"

/**
 * 
 */
UCLASS()
class SLGMODULE_API AGATargetActor_TraceCursor : public AGameplayAbilityTargetActor_Trace
{
	GENERATED_BODY()
	AGATargetActor_TraceCursor();

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
	virtual void StartTargeting(UGameplayAbility* Ability) override;
private:
	bool GetMouseLocationProjectedToNavigation(FHitResult& OutHitResult) const;
};
