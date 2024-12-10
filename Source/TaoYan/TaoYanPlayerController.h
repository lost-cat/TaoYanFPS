// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TaoYanPlayerController.generated.h"

class UInputMappingContext;
DECLARE_EVENT_OneParam(ATaoYanPlayerController,FOnScoreUpdate,float);
/**
 *
 */
UCLASS()
class TAOYAN_API ATaoYanPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;


	// End Actor interface
public:
	// UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreUpdate OnScoreUpdate;
public:
	void GainScore(float Score) const;
	float GetScore() const;
};
