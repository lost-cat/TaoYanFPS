
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

		ATurnBasedCharacterBase* FocusedEnemy = GameMode->GetNextFocusedEnemy();
		if (FocusedEnemy == nullptr)
		{
			return false;
		}
		const ETurnType Turn = GameMode->GetCurrentTurn().TurnType;
		UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		ensure(BlackboardComponent);
		auto SelfActor = Cast<ATurnBasedCharacterBase>(BlackboardComponent->GetValueAsObject("SelfActor"));
		check(SelfActor)
		return SelfActor->IsActionable() && Turn == TargetTurnType && FocusedEnemy == SelfActor;
	}
	return false;
}
