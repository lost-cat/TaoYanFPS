// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskAttackCurrentTarget.h"

#include "TurnBasedCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskAttackCurrentTarget::UBTTaskAttackCurrentTarget()
{
	NodeName = "Attack Current Target";

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskAttackCurrentTarget, BlackboardKey),
	                              ATurnBasedCharacterBase::StaticClass());
}

EBTNodeResult::Type UBTTaskAttackCurrentTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* Object = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName);
	ATurnBasedCharacterBase* CurrentTarget = Cast<ATurnBasedCharacterBase>(Object);
	ATurnBasedCharacterBase* CurrentEnemy = Cast<ATurnBasedCharacterBase>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	if (CurrentTarget && CurrentEnemy)
	{
		CurrentEnemy->Attack(CurrentTarget);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
