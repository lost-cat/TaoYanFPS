// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class TAOYAN_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* UserName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* Password;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* LoginButton;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName LevelToLoad;
private:
	UFUNCTION()
	void OnLoginButtonClicked();
};
