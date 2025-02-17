// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet_CharacterBase.h"

#include "GameplayEffectExtension.h"

void UAttributeSet_CharacterBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
}

void UAttributeSet_CharacterBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (GetHealth()<=0.0f)
	{
		// Handle death
		
	}
	
}
