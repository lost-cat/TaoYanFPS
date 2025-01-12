﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTaskAttackCurrentTarget.generated.h"

/**
 * 
 */
UCLASS()
class SLGMODULE_API UBTTaskAttackCurrentTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTTaskAttackCurrentTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
