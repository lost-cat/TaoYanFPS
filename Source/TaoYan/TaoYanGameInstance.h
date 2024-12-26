// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TaoYanGameInstance.generated.h"

class UTransitionUI;
/**
 * 
 */
UCLASS()
class TAOYAN_API UTaoYanGameInstance : public UGameInstance
{
	GENERATED_BODY()
	UTaoYanGameInstance();

public:
	UFUNCTION(BlueprintCallable)
	void OpenLevel(TSoftObjectPtr<UWorld> Level);
protected:
	virtual void Init() override;
private:
	void TransitionIn();
	void TransitionOut();


private:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,meta=(AllowPrivateAccess="true"))
	UTransitionUI* TransitionUI;
};
