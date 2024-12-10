// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaoYanGameState.h"
#include "GameFramework/GameModeBase.h"
#include "TaoYanGameMode.generated.h"


DECLARE_EVENT_OneParam(ATaoYanGameMode,FOnRemainTimeUpdate,int);
UCLASS(minimalapi)
class ATaoYanGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	
	ATaoYanGameMode();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeDuration = 20.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Classes")
	TSubclassOf<AStaticMeshActor> ScoreCubeClass;
	
	void StartScoreTime();
	

	UFUNCTION(BlueprintCallable)
	void RandomMarkSpecialScoreCube(int Special);

	FOnRemainTimeUpdate OnRemainTimeUpdate;
protected:
	void UpdateTime();
	virtual void BeginPlay() override;

private:
	FTimerHandle TimerHandle;
	
};
