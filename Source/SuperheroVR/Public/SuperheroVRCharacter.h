// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SuperheroVRGameState.h"
#include "SuperheroVRPlayerState.h"
#include "TileSpawner.h"
#include "GameFramework/Character.h"
#include "SuperheroVRCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ASuperheroVRCharacter : public ACharacter
{
	GENERATED_BODY()

protected:	
	
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	// Change to vr arms eventually
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* VR_Gun;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Player State")
	ASuperheroVRPlayerState* CharacterPlayerState;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Game State")	
	ASuperheroVRGameState* GameState;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Location")
	FVector PreviousActorLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Movement")
	float CurrentAcceleration = 1.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Movement")
	float MinAcceleration = 0.9f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Movement")	
	float MaxSpeed = 600.f;

	// 1% speed increase
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float SpeedIncreasePercentage = 1.01f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	ATileSpawner* TileSpawner;

	bool bIsOverlapping = false;

public:

	ASuperheroVRCharacter();

protected:

	virtual void BeginPlay();
	
	virtual void Tick(float DeltaSeconds);	

public:

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

protected:
	
	void OnResetVR();
	
	void MoveForward(float Val);
	
	void MoveRight(float Val);

	void RecordPlayerDistanceTravelled();
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void IncreaseMaxSpeed();

	void ClampPlayerSpeed();

public:

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

