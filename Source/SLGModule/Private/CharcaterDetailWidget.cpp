// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystemComponent.h"
#include "CharacterActionContentWidget.h"
#include "NavigationSystem.h"
#include "NavModifierComponent.h"
#include "TurnBasedCharactor.h"
#include "TurnBasedPlayerController.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Default.h"
#include "Navigation/PathFollowingComponent.h"


void UCharacterActionContentWidget::OnStandbyClicked()
{
	if (CorrespondCharacter == nullptr)
	{
		return;
	}
	CorrespondCharacter->StandBy();

	SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterActionContentWidget::OnMoveClicked()
{
	if (CorrespondCharacter == nullptr || !CorrespondCharacter->CanMove())
	{
		return;
	}
	if (ATurnBasedCharactor* Charactor = Cast<ATurnBasedCharactor>(CorrespondCharacter))
	{
		Charactor->ShowPathIndicator();
	}


	ATurnBasedPlayerController* PlayerController = Cast<ATurnBasedPlayerController>(
		UGameplayStatics::GetPlayerController(this, 0));

	if (PlayerController == nullptr)
	{
		return;
	}
	PlayerController->BindInputMapping(PlayerController->TargetingInputMappingContext, 1);
	ActionsList->SetVisibility(ESlateVisibility::Hidden);
	if (CorrespondCharacter->AbilitySystemComponent->TryActivateAbilityByClass(MoveAbility))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Move Ability Activated"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Move Ability Activation Failed"));
	}
}

void UCharacterActionContentWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Standby->OnClicked.AddUniqueDynamic(this, &UCharacterActionContentWidget::OnStandbyClicked);
	Move->OnClicked.AddUniqueDynamic(this, &UCharacterActionContentWidget::OnMoveClicked);
}


void UCharacterActionContentWidget::OnCorrespondCharacterMoveCompleted(FAIRequestID RequestID,
                                                                       EPathFollowingResult::Type Result)
{
	if (RequestID != FAIRequestID::InvalidRequest)
	{
		SetUIVisibility();
		ActionsList->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCharacterActionContentWidget::OnCharacterSelected(class ATurnBasedCharacterBase* InCharacter)
// inCharacter may be nullptr.
{
	CorrespondCharacter = InCharacter;
	if (CorrespondCharacter == nullptr)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Character %s Selected"), *CorrespondCharacter->GetName()));
	CorrespondCharacter->OnHealthChanged.AddDynamic(this, &UCharacterActionContentWidget::UpdateHP);
	CorrespondCharacter->OnAttackPowerChanged.AddDynamic(this, &UCharacterActionContentWidget::UpdateAttackPower);
	CorrespondCharacter->OnMoveRangeChanged.AddDynamic(this, &UCharacterActionContentWidget::UpdateMoveRange);

	CorrespondCharacter->BroadCastDefaultAttributes();
	SetUIVisibility();
}

void UCharacterActionContentWidget::SetUIVisibility()
{
	ensure(CorrespondCharacter);
	SetVisibility(ESlateVisibility::Visible);
	ActionsList->SetVisibility(CorrespondCharacter->IsActionable()
		                           ? ESlateVisibility::Visible
		                           : ESlateVisibility::Hidden);

	if (ActionsList->GetVisibility() == ESlateVisibility::Visible)
	{
		Move->SetIsEnabled(CorrespondCharacter->CanMove());
		Attack->SetIsEnabled(CorrespondCharacter->CanAttack());
	}
}

void UCharacterActionContentWidget::OnCharacterUnSelected(ATurnBasedCharacterBase* InCharacter)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Character %s UnSelected"), *CorrespondCharacter->GetName()));
	CorrespondCharacter->OnHealthChanged.RemoveDynamic(this, &UCharacterActionContentWidget::UpdateHP);
	CorrespondCharacter->OnAttackPowerChanged.RemoveDynamic(this, &UCharacterActionContentWidget::UpdateAttackPower);
	CorrespondCharacter->OnMoveRangeChanged.RemoveDynamic(this, &UCharacterActionContentWidget::UpdateMoveRange);
	
	CorrespondCharacter = nullptr;
	SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterActionContentWidget::UpdateHP(float NewHP, float OldHP)
{
	
	CharacHP->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), NewHP)));
}

void UCharacterActionContentWidget::UpdateAttackPower(float NewAttackPower, float OldAttackPower)
{
	AttackPower->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), NewAttackPower)));
}

void UCharacterActionContentWidget::UpdateMoveRange(float NewMoveRange, float OldMoveRange)
{
	CharacMoveRange->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), NewMoveRange)));
}
