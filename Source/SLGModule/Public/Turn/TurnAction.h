// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "TurnAction.generated.h"

struct FGameplayTagContainer;
class UTurnAction;
class ATurnBasedCharacterBase;


USTRUCT()
struct FTurnActionHandle
{
	GENERATED_BODY()

	int32 Identifier;
	UTurnAction* Action;
};


/**
 * 
 */
UCLASS(BlueprintType)
class UTurnAction : public UObject
{
	GENERATED_BODY()

public:
	UTurnAction(): TurnIndex(-1), Identifier(-1), BehaviorOwner(nullptr)
	{
	};


	virtual void Execute()
	{
	};

	virtual void Undo()
	{
	};
	/// Begin Getter and Setter
	int32 GetTurnIndex() const
	{
		return TurnIndex;
	}

	void SetTurnIndex(const int32 InTurnIndex)
	{
		this->TurnIndex = InTurnIndex;
	}

	int32 GetIdentifier() const
	{
		return Identifier;
	}

	void SetIdentifier(const int32 InIdentifier)
	{
		this->Identifier = InIdentifier;
	}

	AActor* GetBehaviorOwner() const
	{
		return BehaviorOwner;
	}

	void SetBehaviorOwner(AActor* const InBehaviorOwner)
	{
		this->BehaviorOwner = InBehaviorOwner;
	}

	/// End Getter and Setter

protected:
	int32 TurnIndex;
	int32 Identifier;

	AActor* BehaviorOwner;
};

UCLASS()
class UTurnAction_Move : public UTurnAction
{
	GENERATED_BODY()

public:
	virtual void Execute() override;
	virtual void Undo() override;
	UPROPERTY(BlueprintReadWrite)
	FVector TargetLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector StartLocation;

private:
	ATurnBasedCharacterBase* TargetActor;
};

UCLASS()
class UTurnAction_Attack : public UTurnAction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	ATurnBasedCharacterBase* TargetActor;
	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayAttribute, float> AttributeBeforeActionOnTarget;

	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayAttribute, float> AttributeBeforeActionOnBehaviorOwner;


	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer TagsBeforeActionOnTarget;
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer TagsBeforeActionOnBehaviorOwner;
	// UPROPERTY(BlueprintReadWrite)
	// FGameplayAttribute AttributeToModifyOnTarget;
	// UPROPERTY(BlueprintReadWrite)
	// FGameplayAttribute AttributeToModifyOnBehaviorOwner;


	virtual void Execute() override;
	virtual void Undo() override;
};

UCLASS()
class UTurnAction_Standby : public UTurnAction
{
	GENERATED_BODY()

public:
	virtual void Execute() override;
	virtual void Undo() override;

private:
};
