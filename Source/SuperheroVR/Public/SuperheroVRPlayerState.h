// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SuperheroVRPlayerState.generated.h"

class ASuperheroVRGameState;
/**
 * 
 */
UCLASS()
class SUPERHEROVR_API ASuperheroVRPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Score")
	float FloatScore;
	
	UFUNCTION(Category="Score")
	void IncreaseTotalDistance(float DistanceTravelled);
	
};
