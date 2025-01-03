// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "TaoYanCharacter.h"
#include "TaoYanProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "UMG/MainUIWidget.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	PrimaryComponentTick.TickInterval = 0.1f;
	// auto Object = this->GetOuter();
	// UE_LOG(LogTemp, Warning, TEXT("Outer: %s"), *Object->GetName());
	// auto Owner = this->GetOwner();
	// UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *Owner->GetName());
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr || CurrentBulletCount <= 0)
	{
		return;
	}
	//
	HandleFire();

	// Notify the delegate that the weapon has fired

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		// UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::HandleFire_Implementation()
{
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			auto Projectile = World->SpawnActor<ATaoYanProjectile>(ProjectileClass, SpawnLocation, SpawnRotation,
			                                                       ActorSpawnParams);


			if (Projectile == nullptr)
			{
				return;
			}

			Projectile->SetCharacter(Character);
			// Recoil will up when firing
			CurrentBulletCount--;
			SetRecoil(GetRecoil() + 1.0f);
			OnFired.Broadcast(this);
		}
	}
}

bool UTP_WeaponComponent::AttachWeapon(ATaoYanCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));


	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	//  set the owner of the weapon to the character
	auto Actor = Cast<AActor>(GetOuter());
	Actor->SetOwner(Character);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
	// Bind the weapon to the MainUIWidget
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundWidgets, UMainUIWidget::StaticClass(), true);
	if (FoundWidgets.Num() > 0)
	{
		auto MainWidget = Cast<UMainUIWidget>(FoundWidgets[0]);
		MainWidget->BindEquipWeaponComponent(this);
	}
	return true;
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentBulletCount = MagazineSize;
}

void UTP_WeaponComponent::OnRecoilChange()
{
	OnRecoilChanged.Broadcast(Recoil);
}

void UTP_WeaponComponent::OnRep_Recoil()
{
	OnRecoilChange();
}

void UTP_WeaponComponent::SetRecoil(const float InRecoil)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		Recoil = InRecoil;
		OnRecoilChange();
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::TickComponent(const float DeltaTime, const enum ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Recoil will descend when not firing 
	if (Character != nullptr)
	{
		const auto NewRecoil = FMath::Max(Recoil - 5 * DeltaTime, 0.0f);
		SetRecoil(NewRecoil);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recoil: %f"), Recoil));
	}
}

void UTP_WeaponComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTP_WeaponComponent, Recoil);
	
}
