// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperheroVRCharacter.h"

#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Tile.h"
#include "XRMotionControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

ASuperheroVRCharacter::ASuperheroVRCharacter()
{
	MaxSpeed = 2000.f;
	SpeedIncreasePercentage = 1.0001f;
	MinAcceleration = 0.6f;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 55.f);
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// TODO Attach arms to each motion controller instead of guns
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	// Are motion controllers used if this is false?
	// Uncomment the following line to turn motion controllers on by default:
	bUsingMotionControllers = true;

	bIsOverlapping = false;
}

void ASuperheroVRCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	CharacterPlayerState = Cast<ASuperheroVRPlayerState>(GetPlayerState());
	
	GameState = Cast<ASuperheroVRGameState>(GetWorld()->GetGameState());
	
	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
	
	PreviousActorLocation = GetActorLocation();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileSpawner::StaticClass(), FoundActors);
	TileSpawner = Cast<ATileSpawner>(FoundActors[0]);
}

void ASuperheroVRCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(!GameState->IsGameRunning())
	{
		return;
	}
	
	RecordPlayerDistanceTravelled();
	AddMovementInput(GetActorForwardVector(), 1);

	IncreaseMaxSpeed();
	ClampPlayerSpeed();
}

void ASuperheroVRCharacter::IncreaseMaxSpeed()
{
	MaxSpeed *= SpeedIncreasePercentage;
}

void ASuperheroVRCharacter::ClampPlayerSpeed()
{
	const float DesiredWalkSpeed = MaxSpeed * CurrentAcceleration;
	const float MinSpeed = MaxSpeed*MinAcceleration;
	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(DesiredWalkSpeed, MinSpeed, MaxSpeed);
}

void ASuperheroVRCharacter::RecordPlayerDistanceTravelled()
{	
	const FVector ActorLocation = GetActorLocation();
	float DistanceTravelled = (PreviousActorLocation - ActorLocation).Size();
	DistanceTravelled /= 10;
	CharacterPlayerState->IncreaseTotalDistance(DistanceTravelled);
	PreviousActorLocation = ActorLocation;
}

void ASuperheroVRCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASuperheroVRCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASuperheroVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASuperheroVRCharacter::MoveRight);
}

void ASuperheroVRCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASuperheroVRCharacter::MoveForward(float Value)
{
	if(Value == 0.0f)
	{
		return;
	}

	Value /= 500;

	CurrentAcceleration = FMath::Clamp(CurrentAcceleration + Value, MinAcceleration, 1.f);
}

void ASuperheroVRCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}
