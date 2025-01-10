// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterActionContentWidget.h"
#include "TurnBasedCharactor.h"
#include "TurnBasedPlayerController.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"

#include "Kismet/GameplayStatics.h"
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
		Charactor->ShowNiagaraPath();
	}


	ATurnBasedPlayerController* PlayerController = Cast<ATurnBasedPlayerController>(
		UGameplayStatics::GetPlayerController(this, 0));

	if (PlayerController == nullptr)
	{
		return;
	}
	PlayerController->BindInputMapping(PlayerController->PawnOperationInputMappingContext, 1);
	ActionsList->SetVisibility(ESlateVisibility::Hidden);
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
		UpdateUI();
		ActionsList->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCharacterActionContentWidget::SetCorrespondCharacter(class ATurnBasedCharacterBase* InCharacter)
// inCharacter may be nullptr.
{
	CorrespondCharacter = InCharacter;
	UpdateUI();
}

void UCharacterActionContentWidget::UpdateUI()
{
	if (CorrespondCharacter == nullptr)
	{
		return;
	}
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

void UCharacterActionContentWidget::OnPlayerUnSelected(ATurnBasedCharacterBase* InCharacter)
{
	CorrespondCharacter = nullptr;
	SetVisibility(ESlateVisibility::Hidden);
}
