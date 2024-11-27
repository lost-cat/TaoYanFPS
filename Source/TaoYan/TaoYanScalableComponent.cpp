// Fill out your copyright notice in the Description page of Project Settings.


#include "TaoYanScalableComponent.h"

#include "TaoYanGameMode.h"
#include "TaoYanPlayerController.h"
#include "TaoYanProjectile.h"
#include "GameFramework/Character.h"


// Sets default values
UTaoYanScalableComponent::UTaoYanScalableComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	// PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void UTaoYanScalableComponent::BeginPlay()
{
	Super::BeginPlay();
	SetScaleComponent(GetOwner()->GetComponentByClass<UPrimitiveComponent>());

}


void UTaoYanScalableComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherComp != PrimitiveComponent && OtherComp != nullptr)
	{
		if (const auto Projectile = Cast<ATaoYanProjectile>(OtherActor); Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile Hit"));
			if (const auto Character = Projectile->GetCharacter(); Character)
			{
				if (const auto TaoYanPlayerController = Cast<ATaoYanPlayerController>(Character->Controller))
				{
					const auto GameState = Cast<ATaoYanGameState>(GetWorld()->GetGameState());
					if ( GameState &&
						GameState->IsInScoreTime())
					{
						// auto TaoYanPlayerState = Cast<ATaoYanPlayerState>(Character->GetPlayerState());
						TaoYanPlayerController->GainScore(bIsSpecial ? Score2Get * 2 : Score2Get);
						UE_LOG(LogTemp, Warning, TEXT("Player %s Gained %d points"),
						       *TaoYanPlayerController->GetPawn()->GetName(), bIsSpecial ? Score2Get * 2 : Score2Get);
					}
				}
			}
			// Destroy the projectile if component has already scaled
			if (bIsScaled)
			{
				GetOwner()->Destroy();
			}
			else // scale the component
			{
				const auto CurrentScale = GetOwner()->GetActorScale3D();
				GetOwner()->SetActorScale3D(CurrentScale / ScaleFactor);
				bIsScaled = true;
			}
		}
	}
}

void UTaoYanScalableComponent::SetScaleComponent(UPrimitiveComponent* InPrimitiveComponent)
{
	PrimitiveComponent = InPrimitiveComponent;
	if (PrimitiveComponent)
	{
		PrimitiveComponent->SetSimulatePhysics(true);
		PrimitiveComponent->OnComponentHit.AddDynamic(this, &UTaoYanScalableComponent::OnHit);
	}
}

void UTaoYanScalableComponent::SetSpecial(bool bSpecial)
{
	bIsSpecial = bSpecial;
	OnSetSpecial.Broadcast(bIsSpecial);
}