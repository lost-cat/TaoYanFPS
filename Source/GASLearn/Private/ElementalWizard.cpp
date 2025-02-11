// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalWizard.h"


// Sets default values
AElementalWizard::AElementalWizard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
}

// Called when the game starts or when spawned
void AElementalWizard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElementalWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AElementalWizard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// UAbilitySystemComponent* AElementalWizard::GetAbilitySystemComponent() const
// {
// 	return AbilitySystemComponent;
// }

