// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileSpawner.generated.h"

class ASuperheroVRCharacter;
class ATile;	
struct FPlayerMovement;

USTRUCT(BlueprintType)
struct FRow
{
	GENERATED_BODY()

	TArray<ATile*> Tiles;
};

UENUM(BlueprintType)
enum class Direction: uint8
{
	Forward     UMETA(DisplayName = "Forward"),
	Right       UMETA(DisplayName = "Right"),
	Left	    UMETA(DisplayName = "Left"),
	None        UMETA(DisplayName = "None"),
};

UCLASS()
class SUPERHEROVR_API ATileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileSpawner();
	
	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SpawnTilesInDirection(Direction Direction);

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void DestroyTilesInDirection(Direction Direction);

	UFUNCTION(Category="Spawning")
	void CheckTiles();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tiles")
	TSubclassOf<ATile> TileClass; 
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Tiles")
	ATile* PlayerTile;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Tiles")
	int32 MiddleTileIndex;

	UFUNCTION(BlueprintCallable, Category="Spawning")
	ATile* SpawnTileAtLocation(FVector TileLocation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	int32 DesiredRowCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	int32 TilesPerRow;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	TArray<FRow> Rows;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	float ZSpawnOffset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	float ZSpawnCoordinate;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	ATile* PreviousTile;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Player")
	ASuperheroVRCharacter* Player;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Spawning")
	bool bIsCheckingTiles = false;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
};
