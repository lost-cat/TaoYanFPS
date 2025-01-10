// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TurnBasedCharacterBase.generated.h"

class UTurnBasedCharacterHealthBar;
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
	//Begin GETTER & SETTER
	FORCEINLINE bool IsActionable() const { return bActionable; }
	FORCEINLINE void SetActionable(const bool InbActionable)
	{
		this->bActionable = InbActionable;
	}

	FORCEINLINE float GetMaxDistancePerTurn() const { return MaxDistancePerTurn; }
	FORCEINLINE void SetMaxDistancePerTurn(const float InMaxDistancePerTurn)
	{
		this->MaxDistancePerTurn = InMaxDistancePerTurn;
	}

	FORCEINLINE int GetMoveCount() const { return MoveCount; }
	FORCEINLINE void SetMoveCount(const int InMoveCount)
	{
		this->MoveCount = InMoveCount;
	}

	FORCEINLINE int GetAttackCount() const { return AttackCount; }
	FORCEINLINE void SetAttackCount(const int InAttackCount)
	{
		this->AttackCount = InAttackCount;
	}

	//End GETTER & SETTER


	FORCEINLINE bool CanMove() const { return bActionable && MoveCount > 0; }
	FORCEINLINE bool CanAttack() const { return bActionable && AttackCount > 0; }

	virtual void OnSelected(APlayerController* PlayerController);
	virtual void OnUnSelected(APlayerController* PlayerController);
	virtual void StandBy();
	virtual void Attack(ATurnBasedCharacterBase* Target);
	virtual void OnAttacked(ATurnBasedCharacterBase* Attacker);
	virtual void ResetTurnRelatedState();

private:
	float Health = 100.0f;
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UTurnBasedCharacterHealthBar> HealthBarClass;

	float Speed;
	float MaxDistancePerTurn = 500.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TurnRelatedState", meta=(AllowPrivateAccess="true"))
	bool bActionable = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TurnRelatedState", meta=(AllowPrivateAccess="true"))
	int MoveCount = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TurnRelatedState", meta=(AllowPrivateAccess="true"))
	int AttackCount = 1;
};
