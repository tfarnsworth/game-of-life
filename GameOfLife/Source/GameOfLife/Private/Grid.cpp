// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "ClickableTile.h"
#include "Tile.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	InitializeGrid();
}

void AGrid::InitializeGrid()
{
	int32 Spacer = 0;
	for (int32 RowIndex = 0; RowIndex < GridSize; RowIndex++)
	{
		for (int32 ColumnIndex = 0; ColumnIndex < GridSize; ColumnIndex++)
		{
			const float XOffset = (Spacer / GridSize) * 55; // Divide by dimension
			const float YOffset = (Spacer % GridSize) * 55; // Modulo gives remainder
			Spacer++;

			// Make position vector, offset from Grid location
			const FVector TileLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();
			AClickableTile* TileToCreate = GetWorld()->SpawnActor<AClickableTile>(ClickableTile, TileLocation, FRotator::ZeroRotator);

			if (TileToCreate)
			{
				// Store reference to tile for manipulating populated state later
				Grid[RowIndex][ColumnIndex] = TileToCreate;
				// Initialize boolean map, which is used for storing the old generation of tiles
				PopulatedStates[RowIndex][ColumnIndex] = false;
				auto TileMeshToCreate = TileToCreate->FindComponentByClass<UTile>();
				if (!TileMeshToCreate)
				{
					UE_LOG(LogTemp, Warning, TEXT("Could not find child mesh"));
					break;
				}
				// Pass a reference to the grid coords and grid parent to the tile, so it can notify it's parent grid of updates
				TileMeshToCreate->SetGridCoordinates(RowIndex, ColumnIndex);
				TileMeshToCreate->SetParentGrid(this);
			}
		}
	}
}

void AGrid::StartGame()
{
	GetWorld()->GetTimerManager().SetTimer(GameTimer, this, &AGrid::AdvanceGameState, GameTickSpeed, true, 0.f);
}

void AGrid::PauseGame()
{
	GetWorld()->GetTimerManager().PauseTimer(GameTimer);
}

void AGrid::UpdatePopulatedState(int32 Row, int32 Column, bool Populated)
{
	PopulatedStates[Row][Column] = Populated;
}

void AGrid::AdvanceGameState()
{
	// Freeze the population in it's current state and use for calculations
	bool CurrentPopulatedStates[20][20];
	memcpy(CurrentPopulatedStates, PopulatedStates, sizeof(PopulatedStates));

	for (int32 xIndex = 0; xIndex < GridSize; xIndex++)
	{
		for (int32 yIndex = 0; yIndex < GridSize; yIndex++)
		{
			int32 NeighborCount = 0;

			// Determine the bounds for checking neighbors and prevent checking outside the edges of the grid
			int32 LowerBoundX = xIndex - 1 > 0 ? xIndex - 1 : 0;
			int32 UpperBoundX = xIndex + 1 < GridSize ? xIndex + 1 : GridSize - 1;
			int32 LowerBoundY = yIndex - 1 > 0 ? yIndex - 1 : 0;
			int32 UpperBoundY = yIndex + 1 < GridSize ? yIndex + 1 : GridSize - 1;

			// Loop through neighbors
			for (int32 i = LowerBoundX; i <= UpperBoundX; i++)
			{
				for (int32 j = LowerBoundY; j <= UpperBoundY; j++)
				{
					//Skip the calculation if this is the current tile
					if (i == xIndex && j == yIndex) { continue; }
					if (xIndex == 1 && yIndex == 0) {
						bool TheState = CurrentPopulatedStates[i][j];
					}
					if (CurrentPopulatedStates[i][j] == true)
					{
						NeighborCount++;
					}
				}
			}

			// Get current tile
			AClickableTile* CurrentTile = Grid[xIndex][yIndex];
			UTile* CurrentTileMesh = CurrentTile->FindComponentByClass<UTile>();

			// If the current tile is populated and has less than two or greater than 3 neighbors, it dies
			// Or if the current tile is not populated and has three neighbors, it becomes populated
			// Use the map of boolean states to calculate everything based on the current generation
			if ((CurrentPopulatedStates[xIndex][yIndex] == true && (NeighborCount < 2 || NeighborCount > 3))
			|| (CurrentPopulatedStates[xIndex][yIndex] == false && NeighborCount == 3))
			{
				CurrentTileMesh->TogglePopulated();
			}	
		}
	}
}

void AGrid::ResetGame()
{
	PauseGame();
	
	for (int32 Row = 0; Row < GridSize; Row++)
	{
		for (int32 Column = 0; Column < GridSize; Column++)
		{
			// Reset stored boolean states for population
			PopulatedStates[Row][Column] = false;

			// Reset tile states in grid
			AClickableTile* CurrentTile = Grid[Row][Column];
			UTile* CurrentTileMesh = CurrentTile->FindComponentByClass<UTile>();
			if (CurrentTileMesh->Populated)
			{
				CurrentTileMesh->TogglePopulated();
			}
		}
	}
}



