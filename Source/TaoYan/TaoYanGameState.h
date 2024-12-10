// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TaoYanGameState.generated.h"

/**
 * 
 */
UCLASS()
class TAOYAN_API ATaoYanGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	bool IsInScoreTime() const { return RemainTime > 0.0f; }
	void SetRemainTime(const float InTime) { RemainTime = InTime; }
	float GetRemainTime() const { return RemainTime; }
private:
	UPROPERTY()
	float RemainTime;
};
