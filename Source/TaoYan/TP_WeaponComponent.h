// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class ATaoYanCharacter;
class UTP_WeaponComponent;


DECLARE_EVENT_OneParam(UTP_WeaponComponent, FOnRecoilChanged, float);
DECLARE_EVENT_OneParam(UTP_WeaponComponent, FOnFired, UTP_WeaponComponent*);
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAOYAN_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();
	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(ATaoYanCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();
	UFUNCTION(Server, Reliable)
	void HandleFire();


	UFUNCTION()
	void OnRecoilChange();
	UFUNCTION()
	void OnRep_Recoil();

	void SetRecoil(float InRecoil);
	FORCEINLINE float GetRecoil() const { return Recoil; }
	
protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual  void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ATaoYanProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	

	FOnRecoilChanged OnRecoilChanged;
	FOnFired OnFired;
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	int MagazineSize;
	FORCEINLINE int GetCurrentBulletCount() const { return CurrentBulletCount; }

private:
	/** The Character holding this weapon*/
	ATaoYanCharacter* Character;

	int CurrentBulletCount;
	//  current recoil of the weapon  will up when firing and descend when not firing 
	UPROPERTY(ReplicatedUsing=OnRep_Recoil)
	float Recoil = 0;
};

