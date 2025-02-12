// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "TurnBasedCharacterBase.generated.h"

class UAttributeSet_CharacterBase;
class USphereComponent;
class UAbilitySystemComponent;
class UNavModifierComponent;
class UTurnBasedCharacterHealthBar;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedSignature, float, NewValue,float,OldValue);

UCLASS()
class SLGMODULE_API ATurnBasedCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnPunchSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult);
	// Sets default values for this character's properties
	ATurnBasedCharacterBase();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> PreLoadedAbilities;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Abilities")
	TObjectPtr<UAttributeSet_CharacterBase> AttributeSet;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Collision")
	TObjectPtr<USphereComponent> PunchSphere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Navigation")
	TObjectPtr<UNavModifierComponent> NavModifierComponent;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnAttackPowerChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnMoveRangeChanged;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Begin GETTER & SETTER

	FORCEINLINE TObjectPtr<UNavModifierComponent> GetNavModifierComponent() const { return NavModifierComponent; }
	FORCEINLINE bool IsActionable() const { return bActionable; }
	FORCEINLINE void SetActionable(const bool InbActionable)
	{
		this->bActionable = InbActionable;
	}

	//End GETTER & SETTER


	FORCEINLINE bool CanMove() const { return bActionable && MoveCount > 0; }
	FORCEINLINE bool CanAttack() const { return bActionable && AttackCount > 0; }

	virtual void OnSelected(APlayerController* PlayerController);
	virtual void OnUnSelected(APlayerController* PlayerController);
	
	virtual void StandBy();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="Abilities")
	void Attack(ATurnBasedCharacterBase* Target);
	virtual void Attack_Implementation(ATurnBasedCharacterBase* Target);
	virtual void OnAttacked(ATurnBasedCharacterBase* Attacker);
	virtual void ResetTurnRelatedState();
	UFUNCTION(BlueprintCallable)
	void BeginMoving();
	UFUNCTION(BlueprintCallable)

	void BroadCastDefaultAttributes();
	// void BindAttributeSetToUI();
	// void UnBindAttributeSetToUI();
private:
	UFUNCTION()
	void  UpdateHealthBar(float NewHealth, float OldHealth);

	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
	void OnAttackPowerAttributeChanged(const FOnAttributeChangeData& Data);
	void OnMoveRangeAttributeChanged(const FOnAttributeChangeData& Data);
	
private:


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UTurnBasedCharacterHealthBar> HealthBarClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TurnRelatedState", meta=(AllowPrivateAccess="true"))
	bool bActionable = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TurnRelatedState", meta=(AllowPrivateAccess="true"))
	int MoveCount = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TurnRelatedState", meta=(AllowPrivateAccess="true"))
	int AttackCount = 1;
};
