// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_MoveToTarget.generated.h"

class UNavigationQueryFilter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveToTargetDelegate);
/**
 * 
 */
UCLASS()
class SLGMODULE_API UAbilityTask_MoveToTarget : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_MoveToTarget* MoveToTarget(UGameplayAbility* OwningAbility, FName TaskInstanceName, AActor* TargetActor,const FVector& TargetLocation,
		float AcceptanceRadius = 10.0f, bool bStopOnOverlap = false, bool bUsePathfinding = true, bool bUseAsyncPathfinding = true,
		TSubclassOf<UNavigationQueryFilter> FilterClass = nullptr);

	FMoveToTargetDelegate OnMoveFinished;
	FMoveToTargetDelegate OnMoveFailed;
	virtual void Activate() override;
	
	
};
