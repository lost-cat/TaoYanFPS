// Fill out your copyright notice in the Description page of Project Settings.


#include "BTFindSuitableTarget.h"

#include "TurnBasedEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskFindSuitableTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	check(BlackboardComponent)
	UObject* Object = BlackboardComponent->GetValueAsObject("SelfActor");
	ATurnBasedEnemy* Enemy = Cast<ATurnBasedEnemy>(Object);
	check(Enemy);
	const bool bTarget = Enemy->FindSuitableTarget();
	BlackboardComponent->SetValueAsObject("TargetPlayerControlledPawn", Enemy->GetCurrentTarget());
	return  bTarget ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	
}

UBTTaskFindSuitableTarget::UBTTaskFindSuitableTarget()
{
	NodeName = "Find Suitable Target";
}
