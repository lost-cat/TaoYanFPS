// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCharactor.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TurnBasedAIController.h"
#include "TurnBasedEnemy.h"
#include "TurnBasedPlayerController.h"
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

void ATurnBasedCharactor::ShowMovementRange()
{
}

void ATurnBasedCharactor::ShowOperationContents()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Display, TEXT("ATurnBasedCharactor::ShowOperationContents"));
		if (HitResult.GetActor()->IsA<ATurnBasedEnemy>())
		{
			ATurnBasedEnemy* Enemy = Cast<ATurnBasedEnemy>(HitResult.GetActor());
			// MoveToActorAndAttack(Enemy);
		}
	}
	if (auto AIController = GetController<ATurnBasedAIController>())
	{
		if (AIController->GetMoveStatus() != EPathFollowingStatus::Idle)
		{
			UE_LOG(LogTemp, Warning, TEXT("ATurnBasedCharactor::MoveTo AIController is Moving"));
			return;
		}


		if (PlayerController)
		{
			PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
			if (HitResult.bBlockingHit)
			{
				UE_LOG(LogPathFollowing, Warning, TEXT("ATurnBasedCharactor::MoveTo HitResult"));
				const auto TargetLocation = HitResult.Location;
				// UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TargetLocation);
				AIController->MoveToLocation(TargetLocation, 10.0f, true, true, true);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ATurnBasedCharactor::MoveTo AIController is nullptr"));
	}
}


// void ATurnBasedCharactor::MoveToActorAndAttack(const AActor* TargetActor)
// {
// 	const auto TurnBasedAIController = GetController<ATurnBasedAIController>();
// 	if (TurnBasedAIController == nullptr)
// 	{
// 		return;
// 	}
// 	FAIRequestID FaiRequestID = TurnBasedAIController->MoveToActorIfIdle(
// 		TargetActor, 20.0f, true, true, true, nullptr, true);
// 	if (FaiRequestID == FAIRequestID::InvalidRequest)
// 	{
// 		return;
// 	}
// 	AttackDelegateHandle = TurnBasedAIController->GetPathFollowingComponent()->OnRequestFinished.AddUFunction(
// 		this, "Attack", Cast<ATurnBasedCharacterBase>(TargetActor));
// }

void ATurnBasedCharactor::MoveToLocation(const FVector& TargetLocation)
{
	if (const auto AIController = GetController<ATurnBasedAIController>())
	{
		if (AIController->GetMoveStatus() != EPathFollowingStatus::Idle)
		{
			UE_LOG(LogTemp, Warning, TEXT("ATurnBasedCharactor::MoveTo AIController is Moving"));
			return;
		}
		AIController->MoveToLocation(TargetLocation, 20.0f, true, true, true);
	}
}

// void ATurnBasedCharactor::MoveToCursor()
// {
// 	UE_LOG(LogTemp, Display, TEXT("MoveToCursor"));
// 	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
// 	if (const auto TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController))
// 	{
// 		if (const auto HoverActor = TurnBasedPlayerController->GetHoverActor(); HoverActor && HoverActor->IsA<
// 			ATurnBasedCharacterBase>())
// 		{
// 			//  if the cursor location is near the character, then do not move
// 			return;
// 		}
//
// 		if (FVector CursorLocation; TurnBasedPlayerController->GetCursorLocation(CursorLocation))
// 		{
// 			MoveToLocation(CursorLocation);
// 		}
// 	}
// }

void ATurnBasedCharactor::Attack(ATurnBasedCharacterBase* Target)
{
	Super::Attack(Target);
	UE_LOG(LogTemp, Display, TEXT("Attack %s"), *Target->GetName());
	// const auto TurnBasedAIController = GetController<ATurnBasedAIController>();
	// if (TurnBasedAIController == nullptr)
	// {
	// 	return;
	// }
	// TurnBasedAIController->GetPathFollowingComponent()->OnRequestFinished.Remove(AttackDelegateHandle);
}

// void ATurnBasedCharactor::AttackPawnUnderCursor()
// {
// 	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
// 	const auto TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController);
// 	if (TurnBasedPlayerController == nullptr) // not player controller
// 	{
// 		return;
// 	}
// 	const auto HoverActor = TurnBasedPlayerController->GetHoverActor();
// 	if (HoverActor == nullptr || !HoverActor->IsA<ATurnBasedEnemy>()) // no target actor  under cursor or not enemy
// 	{
// 		return;
// 	}
//
// 	// MoveToActorAndAttack(HoverActor);
// }


void ATurnBasedCharactor::OnSelected(APlayerController* PlayerController)
{
	Super::OnSelected(PlayerController);
	UE_LOG(LogTemp, Display, TEXT("OnSelected Pawn: %s"), *GetName());
}

void ATurnBasedCharactor::OnUnSelected(APlayerController* PlayerController)
{
	Super::OnUnSelected(PlayerController);
	// if (PlayerController)
	// {
	// 	// if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
	// 	// 	PlayerController->InputComponent))
	// 	// {
	// 	// 	bool bRemoveBindingByHandle = EnhancedInputComponent->RemoveBindingByHandle(PawnMoveInputActionHandle);
	// 	//
	// 	// 	UE_LOG(LogTemp, Warning, TEXT("UnSelected and RemoveBinding %d"), bRemoveBindingByHandle);
	// 	// }
	// }
}

void ATurnBasedCharactor::UpdatePathIndicator()
{
	class APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ATurnBasedPlayerController* TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController);
	if (TurnBasedPlayerController == nullptr)
	{
		return;
	}
	FVector CursorLocation;
	TurnBasedPlayerController->GetCursorLocation(CursorLocation);
	auto NavV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	UNavigationPath* Path = NavV1->FindPathToLocationSynchronously(this, GetActorLocation(), CursorLocation);
	if (Path == nullptr)
	{
		return;
	}
	// PathIndicatorComponent->
}

void ATurnBasedCharactor::ShowNiagaraPath()
{
	UNiagaraComponent* SpawnSystemAttached = UNiagaraFunctionLibrary::SpawnSystemAttached(
		PathIndicator.LoadSynchronous(), GetRootComponent(), NAME_None,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTargetIncludingScale, false, true);
	//todo update path every 0.01s
	GetWorld()->GetTimerManager().SetTimer(UpdatePathTimerHandle, this, &ATurnBasedCharactor::UpdatePathIndicator,
	                                       0.01f, true);
}
