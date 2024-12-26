// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedPlayerPawn.h"


#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ATurnBasedPlayerPawn::ATurnBasedPlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(" CursorMesh");
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");

	CameraComponent->SetupAttachment(SpringArmComponent);
	SpringArmComponent->SetupAttachment(RootComponent);
	CursorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATurnBasedPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurnBasedPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATurnBasedPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ATurnBasedPlayerPawn::MoveAround(const FInputActionValue& Value)
{
	AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
	AddMovementInput(GetActorRightVector(), Value.Get<FVector2D>().X);
}

void ATurnBasedPlayerPawn::DOF()
{
	FPostProcessSettings PostProcessSettings;
	\
	PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	PostProcessSettings.bOverride_DepthOfFieldFstop = true;
	PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
	
	PostProcessSettings.DepthOfFieldFstop = 3.0f;
	PostProcessSettings.DepthOfFieldSensorWidth = 150.0f;
	PostProcessSettings.DepthOfFieldFocalDistance =  SpringArmComponent->TargetArmLength;
	
	CameraComponent->PostProcessSettings = PostProcessSettings;
}



void ATurnBasedPlayerPawn::Zoom(const FInputActionValue& Value)
{
	ZoomDirection = Value.Get<float>();
	UpdateZoom();
	DOF();
	
}

void ATurnBasedPlayerPawn::UpdateZoom()
{
	ZoomValue = FMath::Clamp(ZoomValue + 0.01f * ZoomDirection, 0.0f, 1.0f);
	const float Alpha = ZoomCurve->GetFloatValue(ZoomValue);

	const float NewArmLength = FMath::Lerp(800.0f, 40000.0f, ZoomCurve->GetFloatValue(Alpha));
	const float NewSpringArmPitch = FMath::Lerp(-40.0f, -55.0f, Alpha);
	if (SpringArmComponent)
	{
		SpringArmComponent->TargetArmLength = NewArmLength;
		SpringArmComponent->SetRelativeRotation(FRotator{0, NewSpringArmPitch, 0});
	}
	const float NewMovingSpeed =  FMath::Lerp(1000.f, 6000.0f, Alpha);
	if (UFloatingPawnMovement* MovementComponent = Cast<UFloatingPawnMovement>(GetMovementComponent()))
	{
		MovementComponent->MaxSpeed = NewMovingSpeed;
	}
	DOF();
	const float NewFOV  = FMath::Lerp(20.0f, 15.0f, Alpha);
	CameraComponent->SetFieldOfView(NewFOV);
}
