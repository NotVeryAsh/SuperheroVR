// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperheroVRGameState.h"

#include "TileSpawner.h"
#include "Kismet/GameplayStatics.h"

bool ASuperheroVRGameState::IsGameRunning()
{
	return bIsGameRunning;
}

void ASuperheroVRGameState::SetIsGameRunning(const bool bGameRunning)
{
	bIsGameRunning = bGameRunning;
}

void ASuperheroVRGameState::StartGame()
{
	SetIsGameRunning(true);
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}
