// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurnBasedAIController.generated.h"

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
	FPathFollowingRequestResult MoveToActorIfIdle(const AActor* Goal, float AcceptanceRadius = 20.0f,
	                                              bool bStopOnOverlap = true,
	                                              bool bUsePathfinding = true, bool bCanStrafe = false,
	                                              const TSubclassOf<UNavigationQueryFilter>& FilterClass = nullptr,
	                                              bool bAllowPartialPaths = true);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> PawnMoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> AttackAction;
	void MovetoCursor();
	void AttackPawnUnderCursor();

private:
	FAIRequestID AttackMoveRequestID = FAIRequestID::InvalidRequest;
	AActor* ActorToAttack = nullptr;
};
