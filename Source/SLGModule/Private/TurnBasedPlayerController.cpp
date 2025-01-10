// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedPlayerController.h"

#include "CharacterActionContentWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "SLGMainWidget.h"
#include "TurnBasedCharactor.h"
#include "TurnBasedPlayerPawn.h"
#include  "TurnBasedCharacterBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

void ATurnBasedPlayerController::BeginPlay()
{
	Super::BeginPlay();


	BindInputMapping(BaseMovingInputMappingContext, 0);

	MainWidget = Cast<USLGMainWidget>(
		CreateWidget(this, UIMainClass.LoadSynchronous(), TEXT("MainWidget")));
	if (!MainWidget)
	{
		return;
	}
	MainWidget->AddToViewport();
	CharacterActionContentWidget = MainWidget->GetCharacterActionsWidget();
	if (CharacterActionContentWidget)
	{
		// CharacterActionContentWidget->AddToViewport();
		CharacterActionContentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	OnSelectedDelegate.AddUniqueDynamic(CharacterActionContentWidget,
	                                    &UCharacterActionContentWidget::SetCorrespondCharacter);
	OnUnSelectedDelegate.AddUniqueDynamic(CharacterActionContentWidget,
	                                      &UCharacterActionContentWidget::OnPlayerUnSelected);
}

ATurnBasedPlayerController::ATurnBasedPlayerController()
{
	bShowMouseCursor = true;
}

void ATurnBasedPlayerController::SetSelectedPawn(ATurnBasedCharactor* Charactor)
{
	if (SelectedPawn != nullptr)
	{
		UnSelect();
	}
	SelectedPawn = Charactor;
	Charactor->OnSelected(this);
	OnSelectedDelegate.Broadcast(SelectedPawn);
}

void ATurnBasedPlayerController::UnSelect()
{
	SelectedPawn->OnUnSelected(this);
	OnUnSelectedDelegate.Broadcast(SelectedPawn);

	SelectedPawn = nullptr;
}

APawn* ATurnBasedPlayerController::GetSelectedPawn() const
{
	return SelectedPawn.Get();
}

AActor* ATurnBasedPlayerController::GetHoverActor() const
{
	return GetPawn<ATurnBasedPlayerPawn>() ? GetPawn<ATurnBasedPlayerPawn>()->GetHoverActor() : nullptr;
}

bool ATurnBasedPlayerController::GetCursorLocation(FVector& OutLocation) const
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		OutLocation = HitResult.Location;
		return true;
	}
	return false;
}

void ATurnBasedPlayerController::AppendControlledPawn(ATurnBasedCharacterBase* PlayerCharacter)
{
	if (PlayerCharacter == nullptr)
	{
		return;
	}
	ControlledPawns.Add(PlayerCharacter);
}


void ATurnBasedPlayerController::SelectPawn(const FInputActionValue& InputActionValue)
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Camera, false, HitResult);
	if (HitResult.bBlockingHit)
	{
		// Process the hit object
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (ATurnBasedCharactor* Charactor = Cast<ATurnBasedCharactor>(HitActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Charactor: %s"), *Charactor->GetName());
				SetSelectedPawn(Charactor);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
				if (SelectedPawn != nullptr)
				{
					UnSelect();
				}
			}
		}
	}
}

void ATurnBasedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Bind Select Pawn Action"));
		EnhancedInputComponent->BindAction(SelectPawnAction.LoadSynchronous(), ETriggerEvent::Completed, this,
		                                   &ATurnBasedPlayerController::SelectPawn);
	}
}


void ATurnBasedPlayerController::BindInputMapping(const TSoftObjectPtr<UInputMappingContext>& InputMappingContext,
                                                  int32 Priority = 0)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		auto EnhancedInputPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (!BaseMovingInputMappingContext.IsNull())
		{
			UE_LOG(LogTemp, Warning, TEXT("Add Mapping Context: %s"), *InputMappingContext.GetAssetName());
			EnhancedInputPlayerSubsystem->AddMappingContext(InputMappingContext.LoadSynchronous(), Priority);
		}
	}
}

void ATurnBasedPlayerController::RemoveInputMapping(const TSoftObjectPtr<UInputMappingContext>& InputMappingContext)
{
	if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("Remove Mapping Context: %s"), *InputMappingContext.GetAssetName());
		const auto EnhancedInputPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		EnhancedInputPlayerSubsystem->RemoveMappingContext(InputMappingContext.LoadSynchronous());
	}
}

void ATurnBasedPlayerController::OnTurnForwarded(const FTurn& NextTurn)
{
	if (NextTurn.TurnType == ETurnType::PlayerTurn)
	{
		ResetAllControlledPawnStates();
	}
}

void ATurnBasedPlayerController::ResetAllControlledPawnStates()
{
	for (ATurnBasedCharacterBase* Pawn_ : ControlledPawns)
	{
		Pawn_->ResetTurnRelatedState();
	}
}
