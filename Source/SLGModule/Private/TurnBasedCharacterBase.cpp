// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCharacterBase.h"

#include "TurnBasedCharacterHealthBar.h"
#include "Components/WidgetComponent.h"


// Sets default values
ATurnBasedCharacterBase::ATurnBasedCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UTurnBasedCharacterHealthBar> HealthBarClassFinder(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SLGContent/UI/UI_CharacterHealthBar.UI_CharacterHealthBar_C'"));
	HealthBarClass = HealthBarClassFinder.Class;
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetWidgetClass(HealthBarClass);
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetDrawAtDesiredSize(true);
}

// Called when the game starts or when spawned
void ATurnBasedCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (auto UserWidget = Cast<UTurnBasedCharacterHealthBar>(HealthBar->GetUserWidgetObject()))
	{
		OnHealthChanged.AddDynamic(UserWidget, &UTurnBasedCharacterHealthBar::UpdateHealthBarPercent);
	}
	OnHealthChanged.Broadcast(Health / MaxHealth);
}


void ATurnBasedCharacterBase::OnSelected(APlayerController* PlayerController)
{
}

void ATurnBasedCharacterBase::OnUnSelected(APlayerController* PlayerController)
{
}

void ATurnBasedCharacterBase::StandBy()
{
}


void ATurnBasedCharacterBase::Attack(ATurnBasedCharacterBase* Target)
{
	if (Target == nullptr)
	{
		UE_LOG(LogHAL, Error, TEXT("Attack Target is nullptr"));
		return;
	}
	AttackCount--;
	Target->OnAttacked(this);
}

void ATurnBasedCharacterBase::OnAttacked(ATurnBasedCharacterBase* Attacker)
{
	this->Health -= 10.0f;
	OnHealthChanged.Broadcast(Health / MaxHealth);
}

void ATurnBasedCharacterBase::ResetTurnRelatedState()
{
	AttackCount = 1;
	MoveCount = 1;
	bActionable = true;
}
