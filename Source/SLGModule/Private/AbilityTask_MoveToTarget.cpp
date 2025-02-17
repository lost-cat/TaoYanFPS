// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_MoveToTarget.h"

UAbilityTask_MoveToTarget* UAbilityTask_MoveToTarget::MoveToTarget(UGameplayAbility* OwningAbility,
                                                                   FName TaskInstanceName, AActor* TargetActor,
                                                                   const FVector& TargetLocation,
                                                                   float AcceptanceRadius, bool bStopOnOverlap,
                                                                   bool bUsePathfinding, bool bUseAsyncPathfinding,
                                                                   TSubclassOf<UNavigationQueryFilter> FilterClass)
{
	UAbilityTask_MoveToTarget* MyObj = NewAbilityTask<UAbilityTask_MoveToTarget>(OwningAbility, TaskInstanceName);
	return MyObj;
}

void UAbilityTask_MoveToTarget::Activate()
{
	Super::Activate();
}
