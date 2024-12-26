// Fill out your copyright notice in the Description page of Project Settings.


#include "TaoYanEnemy.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATaoYanEnemy::ATaoYanEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATaoYanEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATaoYanEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATaoYanEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATaoYanEnemy::UpdateSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed =  NewSpeed;
}
