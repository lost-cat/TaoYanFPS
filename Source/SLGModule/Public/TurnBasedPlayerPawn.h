// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurnBasedPlayerPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class SLGMODULE_API ATurnBasedPlayerPawn : public APawn
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> CursorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	

public:
	// Sets default values for this pawn's properties
	ATurnBasedPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveAround(const FInputActionValue& Value);
	void DOF();
	void UpdateZoom();
	void Zoom(const FInputActionValue& Value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float ZoomValue = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera)
	float ZoomDirection = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera)
	TSoftObjectPtr<UCurveFloat> ZoomCurve;


private:
};
