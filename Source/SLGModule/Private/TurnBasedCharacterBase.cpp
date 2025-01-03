// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCharacterBase.h"

#include "TurnBasedCharacterHealthBar.h"
#include "Components/WidgetComponent.h"


// Sets default values
ATurnBasedCharacterBase::ATurnBasedCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATurnBasedCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (auto UserWidget = Cast<UTurnBasedCharacterHealthBar>( HealthBar->GetUserWidgetObject()))
	{
		OnHealthChanged.AddDynamic(UserWidget, &UTurnBasedCharacterHealthBar::UpdateHealthBarPercent);
	}
	OnHealthChanged.Broadcast(Health/MaxHealth);
}

// Called every frame
void ATurnBasedCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurnBasedCharacterBase::OnSelected(APlayerController* PlayerController)
{
}

void ATurnBasedCharacterBase::OnUnSelected(APlayerController* PlayerController)
{
}


