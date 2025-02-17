// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/TurnGamePlayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_CharacterBase.h"
#include "GATargetActor_TraceCursor.h"
#include "TurnBasedAIController.h"
#include "TurnBasedCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Tasks/AITask_MoveTo.h"
#include "Turn/TurnManagerSubsystem.h"

// UTurnGamePlayAbility //////////////////////////


void UTurnGamePlayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{
	
	
	if (!bWasCancelled)
	{
		UTurnManagerSubsystem* TurnManager = GetWorld()->GetSubsystem<UTurnManagerSubsystem>();

		UTurnAction* TurnAction = TurnManager->CreateTurnAction(TurnActionClass, ActorInfo->AvatarActor.Get());
		ExtractTurnActionWhenSuccess(TurnAction);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("End Ability 123"));
		
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTurnGamePlayAbility::ExtractTurnActionWhenSuccess(UTurnAction* OutTurnAction)
{
	K2_ExtractTurnActionWhenSuccess(OutTurnAction);
}


// UTurnGamePlayAbility_MoveTo //////////////////////////

UTurnGamePlayAbility_MoveTo::UTurnGamePlayAbility_MoveTo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UTurnGamePlayAbility_MoveTo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilityTask_WaitTargetData* WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetData(
		this, "None", EGameplayTargetingConfirmation::UserConfirmed, AGATargetActor_TraceCursor::StaticClass());
	WaitTargetData->ValidData.AddDynamic(this, &UTurnGamePlayAbility_MoveTo::OnTargetDataReady);
	WaitTargetData->Activate();
}

// void UTurnGamePlayAbility_MoveTo::ExtractTurnActionWhenSuccess(UTurnAction& OutTurnAction) const
// {
// 	Super::ExtractTurnActionWhenSuccess(OutTurnAction);
// 	auto TurnAction_Move = Cast<UTurnAction_Move>(&OutTurnAction);
// 	if (TurnAction_Move)
// 	{
// 		TurnAction_Move->TargetLocation = TargetLocation;
// 		TurnAction_Move->StartLocation = StartLocation;
// 	}
// }


void UTurnGamePlayAbility_MoveTo::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{
	//debug print
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Target Data Ready"));
	
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(Data, 0);
	if (HitResult.bBlockingHit)
	{
		TargetLocation = HitResult.Location;

		auto CharacterBase = Cast<ATurnBasedCharacterBase>(GetAvatarActorFromActorInfo());
		if (CharacterBase)
		{
			StartLocation = CharacterBase->GetActorLocation();
			auto Controller = Cast<ATurnBasedAIController>(CharacterBase->GetController());
			if (Controller)
			{
				auto MaxDistance = GetAbilitySystemComponentFromActorInfo_Checked()->GetNumericAttribute(
					UAttributeSet_CharacterBase::GetMoveDistanceAttribute());
				float CostLength;
				if (Controller->CanMoveToTarget(TargetLocation, MaxDistance, CostLength))
				{
				

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Move To Target"));
					UAITask_MoveTo* Task_MoveTo = UAITask_MoveTo::AIMoveTo(Controller, TargetLocation, nullptr, 20.0f, EAIOptionFlag::Default,
                                   EAIOptionFlag::Type::Disable, true, true, false, EAIOptionFlag::Type::Enable,
                                   EAIOptionFlag::Default);
					Task_MoveTo->ReadyForActivation();
					
					EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true,
					           false);
				}
			}
		}
	}
	else
	{
		// InstancingPolicy =  
		// CancelAbility(GetCurrentAbilitySpecHandle())
	}
}
