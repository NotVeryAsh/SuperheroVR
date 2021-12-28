// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSpawner.h"

#include "SuperheroVRCharacter.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATileSpawner::ATileSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATileSpawner::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), FoundActors);
	ATileSpawner* TileSpawner = Cast<ATileSpawner>(FoundActors[0]);
	TileSpawner->SpawnTilesInDirection(Direction::None);

	Player = Cast<ASuperheroVRCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	MiddleTileIndex = TilesPerRow -1;
	MiddleTileIndex /= 2;
}

void ATileSpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CheckTiles();
}

void ATileSpawner::CheckTiles()
{
	if(bIsCheckingTiles)
	{
		return;
	}
	
	bIsCheckingTiles = true;
	
	if(!Player)
	{
		bIsCheckingTiles = false;
		return;
	}

	TArray<AActor*> Tiles;
	Player->GetOverlappingActors(Tiles, ATile::StaticClass());

	if(Tiles.Num() > 1)
	{
		bIsCheckingTiles = false;
		return;
	}

	ATile* CurrentTile = Cast<ATile>(Tiles[0]);

	if(CurrentTile == PreviousTile)
	{
		bIsCheckingTiles = false;
		return;
	}

	PreviousTile = CurrentTile;
	
	int8 CurrentTileIndex = 0;
	int8 CurrentRowIndex = 0;
	
	for(FRow Row: Rows)
	{
		CurrentTileIndex = Row.Tiles.Find(CurrentTile);

		if(CurrentTileIndex != INDEX_NONE)
		{
			break;
		}
		CurrentRowIndex++;
	}
	
	int8 IndexDirection = CurrentTileIndex - MiddleTileIndex;
	
	if(IndexDirection > 0)
	{
		while(IndexDirection > 0)
		{
			SpawnTilesInDirection(Direction::Right);
			DestroyTilesInDirection(Direction::Right);
			IndexDirection--;
		}
	} else if (IndexDirection < 0)
	{
		while(IndexDirection < 0)
		{
			SpawnTilesInDirection(Direction::Left);
			DestroyTilesInDirection(Direction::Left);
			IndexDirection++;
		}
	}

	if(CurrentRowIndex != 0)
	{
		while(CurrentRowIndex > 0)
		{
			DestroyTilesInDirection(Direction::Forward);
			CurrentRowIndex--;
		}
		SpawnTilesInDirection(Direction::Forward);
	}

	bIsCheckingTiles = false;
}

void ATileSpawner::SpawnTilesInDirection(Direction Direction)
{	
	switch (Direction)
	{
		case Direction::Forward:
			{
				FRow NewRow = FRow();
				FRow LastRow = Rows.Last();
				
				for(const ATile* Tile: LastRow.Tiles)
				{
					NewRow.Tiles.Add(SpawnTileAtLocation(Tile->FrontSpawnPoint));
				}
				
				Rows.Add(NewRow);
			}
		
			break;

		case Direction::Right:
			{
				int8 Index = 0;
				
				for(FRow Row: Rows)
				{
					Row.Tiles.Add(SpawnTileAtLocation(Row.Tiles.Last()->RightSpawnPoint));

					Rows[Index] = Row;
					
					Index++;
				}
			}
			break;

		case Direction::Left:
			{
				int8 Index = 0;
			
				for(FRow Row: Rows)
				{
					Row.Tiles.Insert(SpawnTileAtLocation(Row.Tiles[0]->LeftSpawnPoint), 0);

					Rows[Index] = Row;
					
					Index++;
				}
			}
			break;

		// Game has started - there is no movement / overlap direction
		default:

			const APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		
			FVector Location = Pawn->GetActorLocation();
			Location.Z -= ZSpawnOffset;
			ZSpawnCoordinate = Location.Z;
		
			ATile* FirstTile = SpawnTileAtLocation(Location);
		
			FRow Row = FRow();
			Row.Tiles.Add(FirstTile);

			ATile* NextLeftTile = FirstTile;
			ATile* NextRightTile = FirstTile;
		
			while(TilesPerRow > Row.Tiles.Num())
			{
				NextLeftTile = SpawnTileAtLocation(NextLeftTile->LeftSpawnPoint);
				Row.Tiles.Insert(NextLeftTile, 0);
				
				NextRightTile = SpawnTileAtLocation(NextRightTile->RightSpawnPoint);
				Row.Tiles.Add(NextRightTile);
			}

			Rows.Add(Row);

			FRow PrevRow = Row;
		
			while(DesiredRowCount > Rows.Num())
			{
				FRow CurrentRow = FRow();
				
				for(const ATile* Tile: PrevRow.Tiles)
				{
					CurrentRow.Tiles.Add(SpawnTileAtLocation(Tile->FrontSpawnPoint));
				}
				
				PrevRow = CurrentRow;
				
				Rows.Add(CurrentRow);
			}
			
			break;
	}
}

void ATileSpawner::DestroyTilesInDirection(Direction Direction)
{
	switch (Direction)
	{
	case Direction::Forward:
		{
			FRow FirstRow = Rows[0];				

			for(ATile* Tile: FirstRow.Tiles)
			{
				Tile->Destroy();
			}
				
			Rows.RemoveAt(0);
		}
		
		break;

	case Direction::Right:
		{
			int8 Index = 0;
				
			for(FRow Row: Rows)
			{
				ATile* LeftTile = Row.Tiles[0];
				Row.Tiles.Remove(LeftTile);
				LeftTile->Destroy();

				Rows[Index] = Row;
					
				Index++;
			}
		}
		break;

	case Direction::Left:
		{
			int8 Index = 0;
			
			for(FRow Row: Rows)
			{
				ATile* RightTile = Row.Tiles.Last();
				Row.Tiles.Remove(RightTile);
				RightTile->Destroy();

				Rows[Index] = Row;
					
				Index++;
			}
		}
		break;

	default:

		break;
	}
}

ATile* ATileSpawner::SpawnTileAtLocation(FVector SpawnLocation)
{
	const FRotator Rotation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorRotation();
	const FActorSpawnParameters SpawnParameters;
	SpawnLocation.Z = ZSpawnCoordinate;

	return GetWorld()->SpawnActor<ATile>(TileClass, SpawnLocation, Rotation, SpawnParameters);
}
