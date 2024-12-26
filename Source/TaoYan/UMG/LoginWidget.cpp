// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "TaoYan/TaoYanGameInstance.h"

void ULoginWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// Bind the login button
	LoginButton->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginButtonClicked);
}

void ULoginWidget::OnLoginButtonClicked()
{
	if (UserName->GetText().ToString() == TEXT("admin") && Password->GetText().ToString() == TEXT("admin"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
		
		UTaoYanGameInstance* GameInstance = Cast<UTaoYanGameInstance>(GetGameInstance());
		UE_LOG(LogTemp, Warning, TEXT("GameInstance: %s"), *GameInstance->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Open MainWorld") );
		GameInstance->OpenLevel(this->MainWorld);
		if (GameInstance && this->MainWorld != nullptr)
		{
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
	}
	
}
