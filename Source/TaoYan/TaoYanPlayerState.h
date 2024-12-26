// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TaoYanPlayerState.generated.h"



DECLARE_EVENT_OneParam(ATaoYanPlayerController,FOnScoreUpdate,float);
/**
 * 
 */
UCLASS()
class TAOYAN_API ATaoYanPlayerState : public APlayerState
{
	GENERATED_BODY()
	virtual void OnRep_Score() override;
public:
	FOnScoreUpdate OnScoreUpdate;
};
