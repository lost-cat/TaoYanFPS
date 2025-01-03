// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurnBasedPlayerPawn.generated.h"

class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class USphereComponent;
UCLASS()
class SLGMODULE_API ATurnBasedPlayerPawn : public APawn
{
	GENERATED_BODY()


	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> CursorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite ,meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Collision, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Movement, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputAction> MoveAction;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputAction> ZoomAction;
	

public:
	// Sets default values for this pawn's properties
	ATurnBasedPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	virtual void PossessedBy(AController* NewController) override;
	void UpdateHoverActor(AActor* OtherActor);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void UpdateCursor();
	
	void MoveAround(const FInputActionValue& Value);
	void DOF();
	void UpdateZoom();
	void Zoom(const FInputActionValue& Value);
	
	UCurveFloat* GetLazyLoadedZoomCurve();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float ZoomValue = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera)
	float ZoomDirection = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera)
	TSoftObjectPtr<UCurveFloat> ZoomCurve;

	

private:
	UPROPERTY()
	TObjectPtr<AActor>  HoverActor;
	
	FTimerHandle  CursorUpdateTimerHandle;

};


