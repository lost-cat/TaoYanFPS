// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedAIController.h"

#include "CharacterActionContentWidget.h"
#include "EnhancedInputComponent.h"
#include "NavigationSystem.h"
#include "TurnBasedCharacterBase.h"
#include "TurnBasedCharactor.h"
#include "TurnBasedEnemy.h"
#include "TurnBasedPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


void ATurnBasedAIController::BeginPlay()
{
	Super::BeginPlay();


	// bind pawn control related input action  to the AI controller, it may can be down in the Charactor, but i just put it here
	TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (TurnBasedPlayerController)
	{
		const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(TurnBasedPlayerController->InputComponent);
		EnhancedInputComponent->BindAction(PawnMoveAction.LoadSynchronous(), ETriggerEvent::Completed, this,
		                                   &ATurnBasedAIController::MovetoCursor);
		EnhancedInputComponent->BindAction(AttackAction.LoadSynchronous(), ETriggerEvent::Completed, this,
		                                   &ATurnBasedAIController::AttackPawnUnderCursor);
	}
}

void ATurnBasedAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);


	// broadcast the move completed event, so the UI can be reshow  when pawn  finish the move
	const auto Pawn_ = GetPawn<ATurnBasedCharactor>();
	if (Pawn_)
	{
		Pawn_->OnMoveCompleted.Broadcast(RequestID, Result.Code);
	}

	//  this is for move and attack function, if current stored request id is the same as the request id, and the result is success, then attack the target
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

/**
 *   move to the target actor with the max distance limits , 
 * @param Goal 
 * @param Path 
 * @param MaxDistanceLimit 
 * @param AcceptanceRadius 
 * @return 
 */
FPathFollowingRequestResult ATurnBasedAIController::MoveToActorWithMaxDistanceLimits(
	const AActor* Goal, FNavPathSharedPtr& Path,
	float MaxDistanceLimit, float AcceptanceRadius)
{
	FPathFollowingRequestResult Result;
	auto PathFollowingComponent_ = GetPathFollowingComponent();
	// abort active movement to keep only one request running
	if (PathFollowingComponent_ && PathFollowingComponent_->GetStatus() != EPathFollowingStatus::Idle)
	{
		return Result;
	}

	FAIMoveRequest MoveReq{Goal};
	MoveReq.SetUsePathfinding(true)
	       .SetAllowPartialPath(true)
	       .SetNavigationFilter(DefaultNavigationFilterClass)
	       .SetAcceptanceRadius(AcceptanceRadius).SetReachTestIncludesAgentRadius(true)
	       .SetCanStrafe(true);

	if (!TestCanMoveToTargetWithDistanceLimits(MoveReq, MaxDistanceLimit, Path))
	{
		return Result;
	};
	Result = MoveTo(MoveReq, &Path);
	return Result;
}

FPathFollowingRequestResult ATurnBasedAIController::MoveToLocationWithMaxDistanceLimits(const FVector& Goal,
	FNavPathSharedPtr& Path, float MaxDistanceLimit, float AcceptanceRadius)
{
	FPathFollowingRequestResult Result;
	FAIMoveRequest MoveReq;
	auto PathFollowingComponent_ = GetPathFollowingComponent();
	// abort active movement to keep only one request running
	if (PathFollowingComponent_ && PathFollowingComponent_->GetStatus() != EPathFollowingStatus::Idle)
	{
		return Result;
	}

	MoveReq.SetGoalLocation(Goal);
	MoveReq.SetUsePathfinding(true)
	       .SetAllowPartialPath(true)
	       .SetNavigationFilter(DefaultNavigationFilterClass)
	       .SetAcceptanceRadius(AcceptanceRadius)
	       .SetReachTestIncludesAgentRadius(true)
	       .SetCanStrafe(true);

	FPathFindingQuery PFQuery;

	if (!TestCanMoveToTargetWithDistanceLimits(MoveReq, MaxDistanceLimit, Path))
	{
		return Result;
	};

	Result = MoveTo(MoveReq, &Path);
	return Result;
}


//  move to the cursor location
void ATurnBasedAIController::MovetoCursor()
{
	if (TurnBasedPlayerController == nullptr)
	{
		return;
	}
	// check if the pawn this AI controller controls is selected by player
	if (!IsCurrentPawnSelected(TurnBasedPlayerController))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Pawn is not selected"));

		return;
	}

	// check if the pawn can move
	const auto Pawn_ = GetPawn<ATurnBasedCharacterBase>();
	if (Pawn_ == nullptr || !Pawn_->CanMove())
	{
		return;
	}


	//  if the cursor location is near the character, then do not move
	if (const auto HoverActor = TurnBasedPlayerController->GetHoverActor(); HoverActor && HoverActor->IsA<
		ATurnBasedCharacterBase>())
	{
		return;
	}

	if (FVector CursorLocation; TurnBasedPlayerController->GetCursorLocation(CursorLocation))
	{
		if (GetPathFollowingComponent()->GetStatus() != EPathFollowingStatus::Idle)
		{
			UE_LOG(LogPathFollowing, Warning, TEXT("Pawn %s is Moving"), *Pawn_->GetName());
			return;
		}

		FNavPathSharedPtr Path = MakeShared<FNavigationPath>();
		const auto RequestResult = MoveToLocationWithMaxDistanceLimits(CursorLocation, Path,
		                                                               Pawn_->GetMaxDistancePerTurn());


		// post process for move request
		ATurnBasedCharactor* TurnBasedCharactor = Cast<ATurnBasedCharactor>(Pawn_);
		if (TurnBasedCharactor && RequestResult == EPathFollowingRequestResult::RequestSuccessful)
		// if move request success, and we are moving the player's character then remove the input mapping context and hide the path indicator
		{
			TurnBasedPlayerController->RemoveInputMapping(TurnBasedPlayerController->PawnOperationInputMappingContext);
			TurnBasedCharactor->HideNiagaraPath();
			GetPawn<ATurnBasedCharactor>()->SetMoveCount(GetPawn<ATurnBasedCharactor>()->GetMoveCount() - 1);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can not move to target"));
		}
	}
}

void ATurnBasedAIController::AttackPawnUnderCursor()
{
	check(TurnBasedPlayerController);
	const auto Pawn_ = GetPawn<ATurnBasedCharactor>();
	check(Pawn_);

	if (!IsCurrentPawnSelected(TurnBasedPlayerController))
	// the pawn this AI controller controls is not selected by player
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is not selected"));
		return;
	}
	if (!Pawn_->CanAttack())
	{
		return;
	}
	const auto HoverActor = TurnBasedPlayerController->GetHoverActor();
	check(HoverActor);
	if (!HoverActor->IsA<ATurnBasedEnemy>()) // no target actor  under cursor or not enemy
	{
		return;
	}


	FNavPathSharedPtr Path = MakeShared<FNavigationPath>();
	const FPathFollowingRequestResult Result = MoveToActorWithMaxDistanceLimits(HoverActor, Path,
		Pawn_->GetMaxDistancePerTurn());


	if (Result.Code == EPathFollowingRequestResult::Failed)
	{
		return;
	}


	Pawn_->HideNiagaraPath();
	TurnBasedPlayerController->RemoveInputMapping(TurnBasedPlayerController->PawnOperationInputMappingContext);
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


		GetPawn<ATurnBasedCharactor>()->SetMoveCount(GetPawn<ATurnBasedCharactor>()->GetMoveCount() - 1);
		UE_LOG(LogTemp, Log, TEXT("Move to attack"));
		//Attack after move to the target
		AttackMoveRequestID = Result.MoveId;
		ActorToAttack = HoverActor;
	}
}

bool ATurnBasedAIController::IsCurrentPawnSelected(const ATurnBasedPlayerController* PlayerController) const
{
	if (PlayerController == nullptr)
	{
		return false;
	}

	// if the pawn this AI controller controls is not selected by player , then do not move
	return PlayerController->GetSelectedPawn() ? PlayerController->GetSelectedPawn() == GetPawn() : false;
}

bool ATurnBasedAIController::TestCanMoveToTargetWithDistanceLimits(const FAIMoveRequest& Request,
                                                                   const float MaxDistanceLimit,
                                                                   FNavPathSharedPtr& OutPath) const
{
	FPathFindingQuery PFQuery;
	bool bQuery = BuildPathfindingQuery(Request, PFQuery);
	if (!bQuery)
	{
		return false; // can't find the navi data.
	}
	auto NavV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	const FPathFindingResult FindingResult = NavV1->FindPathSync(PFQuery);
	if (FindingResult.Result != ENavigationQueryResult::Type::Success)
	{
		return false; // can not find path
	}

	if (const auto Length = FindingResult.Path->GetLength(); Length > MaxDistanceLimit)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT(
			       "Can not move to target with distance limits current cost distance is %f,but the max distance limit is %f"
		       ), Length, MaxDistanceLimit);
		return false;
	}
	return true;
}
