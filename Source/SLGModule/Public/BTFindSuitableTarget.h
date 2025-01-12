// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTFindSuitableTarget.generated.h"

/**
 * 
 */
UCLASS()
class SLGMODULE_API UBTTaskFindSuitableTarget : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTaskFindSuitableTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
