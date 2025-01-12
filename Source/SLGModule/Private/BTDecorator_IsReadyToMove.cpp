// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsReadyToMove.h"

#include "TurnBasedGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsReadyToAction::UBTDecorator_IsReadyToAction(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	NodeName = "Is Ready To Move";
	TargetTurnType = ETurnType::PlayerTurn;
}

bool UBTDecorator_IsReadyToAction::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                              uint8* NodeMemory) const
{
	if (ATurnBasedGameMode* GameMode = GetWorld()->GetAuthGameMode<ATurnBasedGameMode>(); ensure(GameMode))
	{
		const ETurnType Turn = GameMode->GetCurrentTurn().TurnType;
		// Turn == TargetTurnType;
		UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		ensure(BlackboardComponent);
		auto SelfActor = Cast<ATurnBasedCharacterBase>(BlackboardComponent->GetValueAsObject("SelfActor"));
		check(SelfActor)
		return SelfActor->IsActionable() && Turn == TargetTurnType;
		
	}
	return false;
}
