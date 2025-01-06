// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedAIController.h"

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
	if (const auto PlayerController = Cast<ATurnBasedPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		// check(PawnMoveAction)
		// check(AttackAction)
		const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
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


void ATurnBasedAIController::MovetoCursor()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ATurnBasedPlayerController* TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController);
	if (TurnBasedPlayerController == nullptr)
	{
		return;
	}
	if (!IsCurrentPawnSelected(TurnBasedPlayerController))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Pawn is not selected"));

		return;
	}

	const auto Pawn_ = GetPawn<ATurnBasedCharacterBase>();
	if (Pawn_ == nullptr)
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

		FNavPathSharedPtr Path = MakeShared<FNavigationPath>();
		auto RequestResult = MoveToLocationWithMaxDistanceLimits(CursorLocation, Path, Pawn_->GetMaxDistancePerTurn());
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
	const auto Pawn_ = GetPawn<ATurnBasedCharacterBase>();
	if (Pawn_ == nullptr)
	{
		return;
	}

	if (!IsCurrentPawnSelected(TurnBasedPlayerController))
	// the pawn this AI controller controls is not selected by player
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is not selected"));
		return;
	}

	const auto HoverActor = TurnBasedPlayerController->GetHoverActor();
	if (HoverActor == nullptr || !HoverActor->IsA<ATurnBasedEnemy>()) // no target actor  under cursor or not enemy
	{
		return;
	}

	FNavPathSharedPtr Path = MakeShared<FNavigationPath>();
	const FPathFollowingRequestResult Result = MoveToActorWithMaxDistanceLimits(HoverActor, Path,
		Pawn_->GetMaxDistancePerTurn());


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
