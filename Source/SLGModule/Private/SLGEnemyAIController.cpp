// Fill out your copyright notice in the Description page of Project Settings.


#include "SLGEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"

void ASLGEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(EnemyBTAsset.LoadSynchronous());
}
