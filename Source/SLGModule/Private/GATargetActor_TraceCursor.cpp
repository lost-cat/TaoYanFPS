// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActor_TraceCursor.h"

#include "Abilities/GameplayAbility.h"
#include "Kismet/GameplayStatics.h"

AGATargetActor_TraceCursor::AGATargetActor_TraceCursor()
{
	
}

FHitResult AGATargetActor_TraceCursor::PerformTrace(AActor* InSourceActor)
{
	FHitResult OutHitResult;
	GetMouseLocationProjectedToNavigation(OutHitResult);
	return OutHitResult;
}

void AGATargetActor_TraceCursor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	PrimaryPC = UGameplayStatics::GetPlayerController(this, 0);
	SourceActor = Ability->GetAvatarActorFromActorInfo();
}

bool AGATargetActor_TraceCursor::GetMouseLocationProjectedToNavigation(FHitResult& OutHitResult) const
{
	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECC_Visibility, true, OutHitResult);
	if (OutHitResult.bBlockingHit)
	{
		return true;
	}
	return false;
}
