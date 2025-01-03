// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "TurnBasedCharactor.h"

ATurnBasedPlayerController::ATurnBasedPlayerController()
{
	bShowMouseCursor = true;
}

void ATurnBasedPlayerController::SetSelectedPawn(ATurnBasedCharactor* Charactor)
{
	SelectedPawn = Charactor;
	Charactor->OnSelected(this);
	
	BindInputMapping(PawnOperationInputMappingContext,1);
}

void ATurnBasedPlayerController::UnSelect()
{
	SelectedPawn->OnUnSelected(this);
	SelectedPawn = nullptr;
	if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("Remove Mapping Context: %s"), *PawnOperationInputMappingContext.GetAssetName());
		const auto EnhancedInputPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		EnhancedInputPlayerSubsystem->RemoveMappingContext(PawnOperationInputMappingContext.LoadSynchronous());
	}
}


void ATurnBasedPlayerController::BindInputMapping(const TSoftObjectPtr<UInputMappingContext>& InputMappingContext, int32 Priority = 0)
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

void ATurnBasedPlayerController::BeginPlay()
{
	Super::BeginPlay();


	BindInputMapping(BaseMovingInputMappingContext);
}


void ATurnBasedPlayerController::SelectPawn(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Log, TEXT("SelectPawn"));
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
