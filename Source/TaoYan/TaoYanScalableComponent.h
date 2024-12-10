// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaoYanScalableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetSpecial, bool, bSpecial);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAOYAN_API UTaoYanScalableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UTaoYanScalableComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void SetScaleComponent(UPrimitiveComponent* InPrimitiveComponent);

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnSetSpecial OnSetSpecial;
	
	UFUNCTION(BlueprintCallable)
	void SetSpecial(bool bSpecial);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	bool bIsSpecial = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score")
	int Score2Get = 5;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scale")
	float ScaleFactor = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scale")
	TObjectPtr<UPrimitiveComponent> PrimitiveComponent;

private:
	// UPROPERTY(Replicated)
	bool bIsScaled = false;
};
