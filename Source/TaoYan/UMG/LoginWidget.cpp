// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

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
		GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, TEXT("Login Success"));
		UGameplayStatics::OpenLevel(this, LevelToLoad, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
	}
	
}
