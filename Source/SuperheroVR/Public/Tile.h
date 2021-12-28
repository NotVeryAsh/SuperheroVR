// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class SUPERHEROVR_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	

	ATile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	UBoxComponent* TriggerVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tile")
	UStaticMeshComponent* Tile; 

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	FVector FrontSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	FVector RightSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	FVector LeftSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	FVector TileSpawnVector;
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
