// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_CharacterBase.h"
#include "NavModifierComponent.h"
#include "TurnBasedCharacterHealthBar.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "NavAreas/NavArea_Null.h"


// Sets default values
ATurnBasedCharacterBase::ATurnBasedCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UTurnBasedCharacterHealthBar> HealthBarClassFinder(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SLGContent/UI/UI_CharacterHealthBar.UI_CharacterHealthBar_C'"));
	HealthBarClass = HealthBarClassFinder.Class;
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetWidgetClass(HealthBarClass);
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetDrawAtDesiredSize(true);

	PunchSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PunchSphere"));
	PunchSphere->SetupAttachment(GetMesh(), "PunchLocation");
	PunchSphere->SetSphereRadius(20.f);
	PunchSphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	PunchSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurnBasedCharacterBase::OnPunchSphereOverlapBegin);

	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifierComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UAttributeSet_CharacterBase>(TEXT("AttributeSet"));
}

void ATurnBasedCharacterBase::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{

	bool bFound;
	auto MaxHealth = AbilitySystemComponent->GetGameplayAttributeValue(UAttributeSet_CharacterBase::GetMaxHealthAttribute(), bFound);
	if (MaxHealth == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Max Health is 0"));
	}
	OnHealthChanged.Broadcast(Data.NewValue, MaxHealth == 0 ? 100 : MaxHealth);
}

void ATurnBasedCharacterBase::OnAttackPowerAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttackPowerChanged.Broadcast(Data.NewValue, Data.OldValue);
}

void ATurnBasedCharacterBase::OnMoveRangeAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnMoveRangeChanged.Broadcast(Data.NewValue, Data.OldValue);
}

void ATurnBasedCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttributeSet_CharacterBase::GetHealthAttribute()).
	                        AddUObject(this, &ATurnBasedCharacterBase::OnHealthAttributeChanged);

	AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(UAttributeSet_CharacterBase::GetFirepowerAttribute()).AddUObject(
			this, &ATurnBasedCharacterBase::OnAttackPowerAttributeChanged);
	AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(UAttributeSet_CharacterBase::GetMoveDistanceAttribute()).AddUObject(
			this, &ATurnBasedCharacterBase::OnMoveRangeAttributeChanged);


	for (TSubclassOf<class UGameplayAbility> Ability : PreLoadedAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec{Ability.GetDefaultObject()});
	}
	OnHealthChanged.AddDynamic(this, &ATurnBasedCharacterBase::UpdateHealthBar);
}

void ATurnBasedCharacterBase::OnPunchSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                        bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this)
	{
		return;
	}
	if (OtherActor->IsA(ATurnBasedCharacterBase::StaticClass()))
	{
		FGameplayEventData GameplayEventData;
		GameplayEventData.Instigator = this;
		GameplayEventData.Target = OtherActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			this, FGameplayTag::RequestGameplayTag("Character.OnAttack.Melee.OnHit"), GameplayEventData);
	}
}


void ATurnBasedCharacterBase::OnSelected(APlayerController* PlayerController)
{
}

void ATurnBasedCharacterBase::OnUnSelected(APlayerController* PlayerController)
{
}

void ATurnBasedCharacterBase::StandBy()
{
	SetActionable(false);
}


void ATurnBasedCharacterBase::Attack_Implementation(ATurnBasedCharacterBase* Target)
{
	if (Target == nullptr)
	{
		UE_LOG(LogHAL, Error, TEXT("Attack Target is nullptr"));
		return;
	}
	// AbilitySystemComponent->TryActivateAbility(FGameplayAbilitySpec{})
	// AttackCount--;
	// Target->OnAttacked(this);
}

void ATurnBasedCharacterBase::OnAttacked(ATurnBasedCharacterBase* Attacker)
{
}

void ATurnBasedCharacterBase::ResetTurnRelatedState()
{
	AttackCount = 1;
	MoveCount = 1;
	bActionable = true;
}

void ATurnBasedCharacterBase::BeginMoving()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ATurnBasedCharacterBase::BroadCastDefaultAttributes()
{
	OnAttackPowerChanged.Broadcast(AbilitySystemComponent->GetNumericAttribute(UAttributeSet_CharacterBase::GetFirepowerAttribute()), 0);
	OnHealthChanged.Broadcast(AbilitySystemComponent->GetNumericAttribute(UAttributeSet_CharacterBase::GetHealthAttribute()), 0);
	OnMoveRangeChanged.Broadcast(AbilitySystemComponent->GetNumericAttribute(UAttributeSet_CharacterBase::GetMoveDistanceAttribute()), 0);
}




void ATurnBasedCharacterBase::UpdateHealthBar(float NewHealth, float MaxHealth)
{
	ensure(MaxHealth);
	auto Percent = NewHealth / MaxHealth;
	if (auto UserWidget = Cast<UTurnBasedCharacterHealthBar>(HealthBar->GetUserWidgetObject()))
	{
		UserWidget->UpdateHealthBarPercent(Percent);
	}
}
