// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedGameMode.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsReadyToMove.generated.h"


/**
 * 
 */
UCLASS()
class SLGMODULE_API UBTDecorator_IsReadyToAction : public UBTDecorator
{
	explicit UBTDecorator_IsReadyToAction(const FObjectInitializer& ObjectInitializer);
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ETurnType TargetTurnType;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
