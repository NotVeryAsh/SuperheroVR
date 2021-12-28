// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SuperheroVRGameState.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHEROVR_API ASuperheroVRGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="State")
	bool IsGameRunning();

	UFUNCTION(BlueprintCallable, Category="State")
	void SetIsGameRunning(const bool bGameRunning);

	UFUNCTION(BlueprintCallable, Category="State")
	void StartGame();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsGameRunning = false;
};
