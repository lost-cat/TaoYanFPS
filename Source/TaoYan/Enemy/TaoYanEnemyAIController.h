// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TaoYanEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TAOYAN_API ATaoYanEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATaoYanEnemyAIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;



private:
	void StartEnemyTimer();
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	// UPROPERTY(EditDefaultsOnly, Category = "AI", BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	// UAIPerceptionComponent* PerceptionComponent;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "AI")
	float LineOfSightTime;
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "AI")
	FName LineOfSightKeyName = "HasLineOfSight";
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "AI")
	FName PlayerCharacterKeyName = "PlayerCharacter";
private:
	FTimerHandle EnemyTimerHandle;	
};
