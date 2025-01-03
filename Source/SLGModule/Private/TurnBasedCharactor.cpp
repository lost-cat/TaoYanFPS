// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCharactor.h"

#include "EnhancedInputComponent.h"
#include "TurnBasedAIController.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
ATurnBasedCharactor::ATurnBasedCharactor()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurnBasedCharactor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurnBasedCharactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurnBasedCharactor::MoveTo()
{
	UE_LOG(LogTemp, Warning, TEXT("ATurnBasedCharactor::MoveTo"));


	if (auto AIController = GetController<ATurnBasedAIController>())
	{
		if (AIController->GetMoveStatus()!= EPathFollowingStatus::Idle)
		{
			UE_LOG(LogTemp, Warning, TEXT("ATurnBasedCharactor::MoveTo AIController is Moving"));
			return;
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			FHitResult  HitResult;
			PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
			if (HitResult.bBlockingHit)
			{  
				UE_LOG(LogPathFollowing, Warning, TEXT("ATurnBasedCharactor::MoveTo HitResult"));
				const auto TargetLocation = HitResult.Location;
				// UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TargetLocation);
				AIController->MoveToLocation(TargetLocation,10.0f,true,true,true);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ATurnBasedCharactor::MoveTo AIController is nullptr"));
	}
}

void ATurnBasedCharactor::Attack(ATurnBasedCharacterBase* Target)
{
	
}


void ATurnBasedCharactor::OnSelected(APlayerController* PlayerController)
{

	if (PlayerController)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnSelected and BindAction"));
			EnhancedInputComponent->RemoveBindingByHandle(PawnMoveInputActionHandle);
			FEnhancedInputActionEventBinding& EnhancedInputActionEventBinding = EnhancedInputComponent->BindAction(
				PawnMoveAction.LoadSynchronous(), ETriggerEvent::Completed,
				this, &ATurnBasedCharactor::MoveTo);

			PawnMoveInputActionHandle = EnhancedInputActionEventBinding.GetHandle();
		}
	}
}

void ATurnBasedCharactor::OnUnSelected(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent))
		{
			bool bRemoveBindingByHandle = EnhancedInputComponent->RemoveBindingByHandle(PawnMoveInputActionHandle);

			UE_LOG(LogTemp, Warning, TEXT("UnSelected and RemoveBinding %d"), bRemoveBindingByHandle);
		}
	}
}
