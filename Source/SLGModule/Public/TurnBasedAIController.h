// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurnBasedAIController.generated.h"

class ATurnBasedPlayerController;
class UInputAction;
/**
 * 
 */
UCLASS()
class SLGMODULE_API ATurnBasedAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

public:
	FPathFollowingRequestResult MoveToActorWithMaxDistanceLimits(const ::AActor* Goal,
	                                                             FNavPathSharedPtr& Path,
	                                                             float MaxDistanceLimit, float AcceptanceRadius = 20.0f
	);

	FPathFollowingRequestResult MoveToLocationWithMaxDistanceLimits(const ::FVector& Goal,
	                                                                FNavPathSharedPtr& Path,
	                                                                float MaxDistanceLimit,
	                                                                float AcceptanceRadius = 20.0f
	);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> PawnMoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> AttackAction;
	void MovetoCursor();
	void AttackPawnUnderCursor();

private:
	bool IsCurrentPawnSelected(const ATurnBasedPlayerController* PlayerController) const;

	/// helper function that check if the pawn can move to the target with the distance limits
	/// @param Request the move request we want to test
	/// @param MaxDistanceLimit  usually the pawn's max distance per turn
	/// @param OutPath  the out corresponding path
	/// @return 
	bool TestCanMoveToTargetWithDistanceLimits(const ::FAIMoveRequest& Request, float MaxDistanceLimit,
	                                            FNavPathSharedPtr& OutPath) const;

private:
	FAIRequestID AttackMoveRequestID = FAIRequestID::InvalidRequest;
	AActor* ActorToAttack = nullptr;

	ATurnBasedPlayerController* TurnBasedPlayerController = nullptr;
};
