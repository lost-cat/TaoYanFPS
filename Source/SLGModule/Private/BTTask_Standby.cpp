// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Standby.h"

#include "TurnBasedCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Standby::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATurnBasedCharacterBase* CurrentActionPawn = Cast<ATurnBasedCharacterBase>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	check(CurrentActionPawn); // should not be nullptr
	CurrentActionPawn->StandBy();
	return EBTNodeResult::Succeeded;
}
