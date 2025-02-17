// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Turn/TurnAction.h"
#include "TurnGamePlayAbility.generated.h"

/**
 *  base gameplay ability for turn based game ,add support for creating turn action when ability success activated which can be used to undo the action
 */
UCLASS()
class SLGMODULE_API UTurnGamePlayAbility : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void ExtractTurnActionWhenSuccess(UTurnAction* OutTurnAction);
	UFUNCTION(BlueprintImplementableEvent,Category = "Turn", DisplayName = "ExtractTurnActionWhenSuccess", meta=(ScriptName = "ExtractTurnActionWhenSuccess"))
	void K2_ExtractTurnActionWhenSuccess(UTurnAction* OutTurnAction);
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Turn")
	TSubclassOf<UTurnAction> TurnActionClass;
};


UCLASS()
class UTurnGamePlayAbility_MoveTo : public UTurnGamePlayAbility
{
	GENERATED_BODY()
	UTurnGamePlayAbility_MoveTo();

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data);

private:
	FVector TargetLocation;
	FVector StartLocation;
};
