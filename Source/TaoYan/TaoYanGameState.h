// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TaoYanGameState.generated.h"

/**
 * 
 */
DECLARE_EVENT_OneParam(ATaoYanGameState,FOnRemainTimeUpdate,int);
UCLASS()
class TAOYAN_API ATaoYanGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	bool IsInScoreTime() const { return RemainTime > 0.0f; }
	
	void SetRemainTime(const float InTime);
	float GetRemainTime() const { return RemainTime; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_RemainTime();
	UFUNCTION()
	void OnRemainTimeUpdate();
public:
	FOnRemainTimeUpdate OnRemainTimeUpdated;
private:
	UPROPERTY(ReplicatedUsing  = OnRep_RemainTime)
	float RemainTime;

	

};
