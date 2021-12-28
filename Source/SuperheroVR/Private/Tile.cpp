// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Tile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	RootComponent = Tile;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	FVector Origin;
	FVector Extent;
	float Radius;
	
	UKismetSystemLibrary::GetComponentBounds(Tile, Origin, Extent, Radius);

	const FVector PointOne = Origin + Extent;
	TileSpawnVector = (PointOne - Origin) * 2;

	// Adjust height so other tiles do not spawn at half of the height of the other tiles
	FrontSpawnPoint = GetActorForwardVector() * TileSpawnVector + Origin;
	LeftSpawnPoint = GetActorRightVector() * -1 * TileSpawnVector + Origin;
	RightSpawnPoint = GetActorRightVector() * TileSpawnVector + Origin;
}

