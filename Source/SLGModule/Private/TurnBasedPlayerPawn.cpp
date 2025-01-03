// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedPlayerPawn.h"


#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "TurnBasedPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATurnBasedPlayerPawn::ATurnBasedPlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	RootComponent = SceneComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(" CursorMesh");
	CursorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	SpringArmComponent->SetupAttachment(RootComponent);
	CursorMesh->SetupAttachment(RootComponent);
	SphereComponent->SetupAttachment(RootComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

// Called when the game starts or when spawned
void ATurnBasedPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	UpdateZoom();
	// LoadObject<UCurveFloat>(ZoomCurve)
}

UPawnMovementComponent* ATurnBasedPlayerPawn::GetMovementComponent() const
{
	return FloatingPawnMovement;
}

void ATurnBasedPlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	GetWorldTimerManager().SetTimer(CursorUpdateTimerHandle, this, &ATurnBasedPlayerPawn::UpdateCursor, 0.01f, true);
}

void ATurnBasedPlayerPawn::UpdateHoverActor(AActor* OtherActor)
{
	if (OtherActor)
	{
		HoverActor = OtherActor;
	}
	TArray<AActor*> OverlappingActors;
	SphereComponent->GetOverlappingActors(OverlappingActors, TSubclassOf<AActor>());
	AActor* ClosestActor = nullptr;
	for (AActor* OverlappingActor : OverlappingActors)
	{
		float Distance = FVector::Dist(OverlappingActor->GetActorLocation(), SphereComponent->GetComponentLocation());
		if (ClosestActor == nullptr || Distance < FVector::Dist(ClosestActor->GetActorLocation(),
		                                                        SphereComponent->GetComponentLocation()))
		{
			ClosestActor = OverlappingActor;
		}
	}

	if (ClosestActor != HoverActor)
	{
		HoverActor = ClosestActor;
	}
}

void ATurnBasedPlayerPawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UpdateHoverActor(OtherActor);
}

void ATurnBasedPlayerPawn::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	TArray<AActor*> OverlappingActors;
	SphereComponent->GetOverlappingActors(OverlappingActors, TSubclassOf<AActor>());
	if (OverlappingActors.Num() == 0)
	{
		HoverActor = nullptr;
	}
	else
	{
		UpdateHoverActor(nullptr);
	}
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
	auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this,
	                                   &ATurnBasedPlayerPawn::MoveAround);
	EnhancedInputComponent->BindAction(ZoomAction.LoadSynchronous(), ETriggerEvent::Triggered, this,
	                                   &ATurnBasedPlayerPawn::Zoom);
}

void ATurnBasedPlayerPawn::UpdateCursor()
{
	ATurnBasedPlayerController* PlayerController = GetController<ATurnBasedPlayerController>();
	FVector CursorLocation;
	FTransform NewCursorTransform;
	if (PlayerController)
	{
		float MousePosX, MousePosY;
		PlayerController->GetMousePosition(MousePosX, MousePosY);
		FVector WorldLocation, WorldDirection;
		PlayerController->DeprojectScreenPositionToWorld(MousePosX, MousePosY, WorldLocation, WorldDirection);
		const auto Intersection = FMath::LinePlaneIntersection(WorldLocation,
															   WorldLocation + WorldDirection * 10000.0f,
															   FVector{0, 0, 0}, FVector{0, 0, 1});
		CursorLocation = Intersection + FVector{0, 0, 10};
		SphereComponent->SetWorldLocation(CursorLocation);
		
		if (HoverActor)
		{
			FVector Origin, Extent;
			HoverActor->GetActorBounds(true, Origin, Extent);
			CursorLocation = FVector{Origin.X, Origin.Y, 20};
			NewCursorTransform.SetLocation(CursorLocation);
			const double NewScale = FVector2D{Extent.X, Extent.Y}.GetAbsMax() / 50.f + 0.25 * FMath::Sin(
					GetWorld()->GetTimeSeconds() * 5) +
				1.0f;
			NewCursorTransform.SetScale3D(FVector{NewScale, NewScale, 1.0f});

			
		}
		else
		{
			
			NewCursorTransform.SetLocation(CursorLocation);
			NewCursorTransform.SetScale3D(FVector{1.0f, 1.0f, 1.0f});
		}
		
		FTransform Transform = UKismetMathLibrary::TInterpTo(CursorMesh->GetComponentTransform(), NewCursorTransform, GetWorld()->DeltaTimeSeconds, 12.0f);
		CursorMesh->SetWorldTransform(Transform);
	}
}


void ATurnBasedPlayerPawn::MoveAround(const FInputActionValue& Value)
{
	// UE_LOG(LogTemp, Warning, TEXT("MoveAround"));
	auto Y = Value.Get<FVector2D>().Y;
	auto X = Value.Get<FVector2D>().X;
	// UE_LOG(LogTemp, Log, TEXT("X: %f, Y: %f"), X, Y);
	AddMovementInput(GetActorForwardVector(), Y);
	AddMovementInput(GetActorRightVector(), X);
}

void ATurnBasedPlayerPawn::DOF()
{
	FPostProcessSettings PostProcessSettings;
	PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	PostProcessSettings.bOverride_DepthOfFieldFstop = true;
	PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;

	PostProcessSettings.DepthOfFieldFstop = 3.0f;
	PostProcessSettings.DepthOfFieldSensorWidth = 150.0f;
	PostProcessSettings.DepthOfFieldFocalDistance = SpringArmComponent->TargetArmLength;

	CameraComponent->PostProcessSettings = PostProcessSettings;
}


void ATurnBasedPlayerPawn::Zoom(const FInputActionValue& Value)
{
	// UE_LOG(LogTemp, Log, TEXT("Zoom"));
	ZoomDirection = Value.Get<float>();
	UpdateZoom();
	DOF();
}

void ATurnBasedPlayerPawn::UpdateZoom()
{
	ZoomValue = FMath::Clamp(ZoomValue + 0.01f * ZoomDirection, 0.0f, 1.0f);
	auto LoadedZoomCurve = GetLazyLoadedZoomCurve();
	if (!LoadedZoomCurve)
	{
		UE_LOG(LogTemp, Error, TEXT("Zoom Curve is not loaded"));
		return;
	}

	const float Alpha = LoadedZoomCurve->GetFloatValue(ZoomValue);

	const float NewArmLength = FMath::Lerp(800.0f, 8000.0f, Alpha);
	const float NewSpringArmPitch = FMath::Lerp(-40.0f, -55.0f, Alpha);
	if (SpringArmComponent)
	{
		SpringArmComponent->TargetArmLength = NewArmLength;
		SpringArmComponent->SetRelativeRotation(FRotator{NewSpringArmPitch, 0, 0});
	}
	const float NewMovingSpeed = FMath::Lerp(1000.f, 6000.0f, Alpha);
	if (UFloatingPawnMovement* MovementComponent = Cast<UFloatingPawnMovement>(GetMovementComponent()))
	{
		MovementComponent->MaxSpeed = NewMovingSpeed;
	}
	DOF();
	const float NewFOV = FMath::Lerp(20.0f, 15.0f, Alpha);
	CameraComponent->SetFieldOfView(NewFOV);
}

inline UCurveFloat* ATurnBasedPlayerPawn::GetLazyLoadedZoomCurve()
{
	if (ZoomCurve.IsPending())
	{
		return ZoomCurve.LoadSynchronous();
	}
	return ZoomCurve.Get();
}
