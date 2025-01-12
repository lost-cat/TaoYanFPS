// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SLGEnemyAIController.generated.h"


class UBehaviorTree;
/**
 * 
 */
UCLASS()
class SLGMODULE_API ASLGEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UBehaviorTree> EnemyBTAsset;

protected:
	virtual void BeginPlay() override;
};
