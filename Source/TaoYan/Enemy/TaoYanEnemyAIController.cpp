// Fill out your copyright notice in the Description page of Project Settings.


#include "TaoYanEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



ATaoYanEnemyAIController::ATaoYanEnemyAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATaoYanEnemyAIController::OnTargetPerceptionUpdated);
	
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bind  OnTargetPerceptionUpdated"));
	UE_LOG(LogTemp, Warning, TEXT("Bind  OnTargetPerceptionUpdated"));
}

void ATaoYanEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ATaoYanEnemyAIController::OnTargetPerceptionUpdated);
}

void ATaoYanEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RUN BehaviorTree "));
	RunBehaviorTree(BehaviorTree);
}




void ATaoYanEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionUpdated"));
	// GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("OnTargetPerceptionUpdated"));
	bool bActorHasTag = Actor->ActorHasTag("Player");
	if (Stimulus.WasSuccessfullySensed() && bActorHasTag)
	{
		GetWorld()->GetTimerManager().ClearTimer(EnemyTimerHandle);
		EnemyTimerHandle.Invalidate();
		GetBlackboardComponent()->SetValueAsBool(LineOfSightKeyName, true);
		GetBlackboardComponent()->SetValueAsObject(PlayerCharacterKeyName, Actor);
	}
	else
	{
		// LineOfSightTime seconds after losing sight of the player, the enemy will stop chasing the player
		GetWorld()->GetTimerManager().SetTimer(EnemyTimerHandle,this, &ATaoYanEnemyAIController::StartEnemyTimer, LineOfSightTime, false);
	}
	
	 
}


void ATaoYanEnemyAIController::StartEnemyTimer()
{
	GetBlackboardComponent()->SetValueAsBool(LineOfSightKeyName, false);
	GetBlackboardComponent()->SetValueAsObject(PlayerCharacterKeyName, nullptr);
}