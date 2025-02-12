// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_CharacterBase.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class SLGMODULE_API UAttributeSet_CharacterBase : public UAttributeSet
{
	GENERATED_BODY()
public:
	// Health attribute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSet_CharacterBase, Health)

	// Max attribute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData MaxHealth = 100;
	ATTRIBUTE_ACCESSORS(UAttributeSet_CharacterBase, MaxHealth)

	// Firepower attribute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData Firepower;
	ATTRIBUTE_ACCESSORS(UAttributeSet_CharacterBase, Firepower)

	// Defensive Power attribute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData DefensivePower;
	ATTRIBUTE_ACCESSORS(UAttributeSet_CharacterBase, DefensivePower)

	// Move Distance attribute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData MoveDistance;
	ATTRIBUTE_ACCESSORS(UAttributeSet_CharacterBase, MoveDistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData AttackCount;
	ATTRIBUTE_ACCESSORS(UAttributeSet_CharacterBase, AttackCount)
};
