// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperheroVRPlayerState.h"

void ASuperheroVRPlayerState::IncreaseTotalDistance(float DistanceTravelled)
{
	SetScore(GetScore() + DistanceTravelled);
}