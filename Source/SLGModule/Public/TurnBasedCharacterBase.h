// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TurnBasedCharacterBase.generated.h"

class UWidgetComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

UCLASS()
class SLGMODULE_API ATurnBasedCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATurnBasedCharacterBase();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnMaxHealthChanged;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual  void OnSelected(APlayerController* PlayerController);
	virtual  void OnUnSelected(APlayerController* PlayerController);
	virtual void Attack(ATurnBasedCharacterBase* Target);
	virtual  void OnAttacked(ATurnBasedCharacterBase* Attacker);

private:
	
	float Health = 100.0f;
	float MaxHealth = 100.0f;
};


