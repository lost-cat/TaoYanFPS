// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedAIController.h"

#include "EnhancedInputComponent.h"
#include "TurnBasedCharacterBase.h"
#include "TurnBasedCharactor.h"
#include "TurnBasedEnemy.h"
#include "TurnBasedPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "Navigation/PathFollowingComponent.h"


void ATurnBasedAIController::BeginPlay()
{
	Super::BeginPlay();
	if (const auto PlayerController = Cast<ATurnBasedPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		EnhancedInputComponent->BindAction(PawnMoveAction.LoadSynchronous(), ETriggerEvent::Completed, this,
		                                   &ATurnBasedAIController::MovetoCursor);
		EnhancedInputComponent->BindAction(AttackAction.LoadSynchronous(), ETriggerEvent::Completed, this,
		                                   &ATurnBasedAIController::AttackPawnUnderCursor);
	}
}

void ATurnBasedAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (RequestID == AttackMoveRequestID && ActorToAttack != nullptr && Result.Code == EPathFollowingResult::Success)
	{
		const auto TurnBasedCharactor = GetPawn<ATurnBasedCharactor>();
		const auto TargetActor = Cast<ATurnBasedCharacterBase>(ActorToAttack);
		if (TargetActor == nullptr && TurnBasedCharactor == nullptr)
		{
			return;
		}
		
		TurnBasedCharactor->Attack(Cast<ATurnBasedCharacterBase>(TargetActor));

		// reset the attack request
		AttackMoveRequestID = FAIRequestID::InvalidRequest;
		ActorToAttack = nullptr;
	}
}

FPathFollowingRequestResult ATurnBasedAIController::MoveToActorIfIdle(const AActor* Goal, const float AcceptanceRadius,
                                                                      const bool bStopOnOverlap,
                                                                      const bool bUsePathfinding, const bool bCanStrafe,
                                                                      const TSubclassOf<UNavigationQueryFilter>&
                                                                      FilterClass,
                                                                      const bool bAllowPartialPaths)
{
	auto PathFollowingComponent_ = GetPathFollowingComponent();
	// abort active movement to keep only one request running
	if (PathFollowingComponent_ && PathFollowingComponent_->GetStatus() != EPathFollowingStatus::Idle)
	{
		return FPathFollowingRequestResult{};
	}

	FAIMoveRequest MoveReq(Goal);
	MoveReq.SetUsePathfinding(bUsePathfinding);
	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
	MoveReq.SetNavigationFilter(*FilterClass ? FilterClass : DefaultNavigationFilterClass);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
	MoveReq.SetCanStrafe(bCanStrafe);


	return MoveTo(MoveReq);
}

void ATurnBasedAIController::MovetoCursor()
{
	const auto Pawn_ = GetPawn<ATurnBasedCharacterBase>();
	if (Pawn_ == nullptr)
	{
		return;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ATurnBasedPlayerController* TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController);
	if (TurnBasedPlayerController == nullptr)
	{
		return;
	}
	if (const auto HoverActor = TurnBasedPlayerController->GetHoverActor(); HoverActor && HoverActor->IsA<
		ATurnBasedCharacterBase>())
	{
		//  if the cursor location is near the character, then do not move
		return;
	}

	if (FVector CursorLocation; TurnBasedPlayerController->GetCursorLocation(CursorLocation))
	{
		if (GetPathFollowingComponent()->GetStatus() != EPathFollowingStatus::Idle)
		{
			UE_LOG(LogPathFollowing, Warning, TEXT("Pawn %s is Moving"), *Pawn_->GetName());
			return;
		}
		MoveToLocation(CursorLocation);
	}
}

void ATurnBasedAIController::AttackPawnUnderCursor()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	const auto TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController);
	if (TurnBasedPlayerController == nullptr) // not player controller
	{
		return;
	}
	const auto HoverActor = TurnBasedPlayerController->GetHoverActor();
	if (HoverActor == nullptr || !HoverActor->IsA<ATurnBasedEnemy>()) // no target actor  under cursor or not enemy
	{
		UE_LOG(LogTemp, Log, TEXT("Directly attack"));

		return;
	}


	FPathFollowingRequestResult Result = MoveToActorIfIdle(HoverActor);


	if (Result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Directly attack
		UE_LOG(LogTemp, Log, TEXT("Directly attack"));
		GetPawn<ATurnBasedCharactor>()->Attack(Cast<ATurnBasedCharacterBase>(HoverActor));
	}
	else
	{
		if (Result.MoveId == FAIRequestID::InvalidRequest)
		{
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Move to attack"));
		//Attack after move to the target
		AttackMoveRequestID = Result;
		ActorToAttack = HoverActor;
	}


	// MoveToActorAndAttack(HoverActor);
}
