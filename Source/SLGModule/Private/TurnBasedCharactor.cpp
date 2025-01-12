// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCharactor.h"

#include "CharacterActionContentWidget.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
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
	PrimaryActorTick.bCanEverTick = false;
	// ActionContentComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ActionContentWidget"));
	// ActionContentComponent->SetupAttachment(GetRootComponent());
	// ActionContentComponent->SetWidgetSpace(EWidgetSpace::Screen);
	// ActionContentComponent->SetDrawAtDesiredSize(true);
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
}


void ATurnBasedCharactor::OnSelected(APlayerController* PlayerController)
{
	Super::OnSelected(PlayerController);
	UE_LOG(LogTemp, Display, TEXT("OnSelected Pawn: %s"), *GetName());

	if (const ATurnBasedPlayerController* TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController))
	{
		OnMoveCompleted.AddUniqueDynamic(TurnBasedPlayerController->GetCharacterActionContentWidget(),
									  &UCharacterActionContentWidget::OnCorrespondCharacterMoveCompleted);
	}
	
}

void ATurnBasedCharactor::OnUnSelected(APlayerController* PlayerController)
{
	Super::OnUnSelected(PlayerController);
	
	if (const ATurnBasedPlayerController* TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController))
	{
		OnMoveCompleted.RemoveDynamic(TurnBasedPlayerController->GetCharacterActionContentWidget(),
									  &UCharacterActionContentWidget::OnCorrespondCharacterMoveCompleted);
	}
}

void ATurnBasedCharactor::StandBy()
{
	Super::StandBy();
	// APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	// ATurnBasedPlayerController* TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController);
}

void ATurnBasedCharactor::ResetTurnRelatedState()
{
	Super::ResetTurnRelatedState();
	
}

void ATurnBasedCharactor::UpdatePathIndicator()
{
	class APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ATurnBasedPlayerController* TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController);
	if (TurnBasedPlayerController == nullptr || PathIndicatorComponent == nullptr)
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
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(PathIndicatorComponent,TEXT("TargetPath"),
	                                                                 Path->PathPoints);
	// PathIndicatorComponent->
}

void ATurnBasedCharactor::ShowNiagaraPath()
{
	if (PathIndicatorComponent != nullptr) // first time init path indicator
	{
		PathIndicatorComponent->DestroyComponent();
		PathIndicatorComponent = nullptr;
	}
	PathIndicatorComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		PathIndicator.LoadSynchronous(), GetRootComponent(), NAME_None,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTargetIncludingScale, false, true);

	if (UpdatePathTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(UpdatePathTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(UpdatePathTimerHandle, this, &ATurnBasedCharactor::UpdatePathIndicator,
		                                       0.01f, FTimerManagerTimerParameters{true, true});
	}
}

void ATurnBasedCharactor::HideNiagaraPath()
{
	if (PathIndicatorComponent)
	{
		// PathIndicatorComponent->Deactivate();
		PathIndicatorComponent->DestroyComponent();
		PathIndicatorComponent = nullptr;
		GetWorld()->GetTimerManager().PauseTimer(UpdatePathTimerHandle);
	}
}
