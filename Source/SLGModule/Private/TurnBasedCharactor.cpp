// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCharactor.h"

#include "CharacterActionContentWidget.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "TurnBasedAIController.h"
#include "TurnBasedPlayerController.h"
#include "Kismet/GameplayStatics.h"

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


void ATurnBasedCharactor::Attack_Implementation(ATurnBasedCharacterBase* Target)
{
	Super::Attack_Implementation(Target);
	UE_LOG(LogTemp, Display, TEXT("Attack %s"), *Target->GetName());
}


void ATurnBasedCharactor::OnSelected(APlayerController* PlayerController)
{
	Super::OnSelected(PlayerController);
	UE_LOG(LogTemp, Display, TEXT("OnSelected Pawn: %s"), *GetName());

	if (const ATurnBasedPlayerController* TurnBasedPlayerController = Cast<ATurnBasedPlayerController>(PlayerController))
	{
		ATurnBasedAIController* AIController = Cast<ATurnBasedAIController>(Controller);
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

void ATurnBasedCharactor::ShowPathIndicator()
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

void ATurnBasedCharactor::HidePathIndicator()
{
	if (PathIndicatorComponent)
	{
		// PathIndicatorComponent->Deactivate();
		PathIndicatorComponent->DestroyComponent();
		PathIndicatorComponent = nullptr;
		GetWorld()->GetTimerManager().PauseTimer(UpdatePathTimerHandle);
	}
}
