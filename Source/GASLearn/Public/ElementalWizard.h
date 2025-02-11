// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Character.h"
#include "ElementalWizard.generated.h"

UCLASS()
class GASLEARN_API AElementalWizard : public ACharacter
{
	GENERATED_BODY()

public:
	AElementalWizard();

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	// UAbilitySystemComponent*  AbilitySystemComponent;
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Abilities")
	// TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
