// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Engine/World.h"
#include "Square.h"

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
			const FVector SquareLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();
			ASquare* SquareToCreate = GetWorld()->SpawnActor<ASquare>(Square, SquareLocation, FRotator::ZeroRotator);

			if (SquareToCreate)
			{
				// Store reference to tile for manipulating populated state later
				UE_LOG(LogTemp, Warning, TEXT("Spawning square into %i %i at location %s"), RowIndex, ColumnIndex, *SquareLocation.ToString());
				Grid[RowIndex][ColumnIndex] = SquareToCreate;
				// Initialize boolean map, which is used for storing the old generation of tiles
				PopulatedStates[RowIndex][ColumnIndex] = false;
				// Pass a reference to the grid coords and grid parent to the tile, so it can notify it's parent grid of updates
				SquareToCreate->SetGridCoordinates(RowIndex, ColumnIndex);
				SquareToCreate->SetParentGrid(this);
			}
		}
	}
}

void AGrid::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Start the game timer"));
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

	for (int32 RowIndex = 0; RowIndex < GridSize; RowIndex++)
	{
		for (int32 ColumnIndex = 0; ColumnIndex < GridSize; ColumnIndex++)
		{
			int32 NeighborCount = 0;

			// Determine the bounds for checking neighbors and prevent checking outside the edges of the grid
			int32 LowerBoundX = RowIndex - 1 > 0 ? RowIndex - 1 : 0;
			int32 UpperBoundX = RowIndex + 1 < GridSize ? RowIndex + 1 : GridSize - 1;
			int32 LowerBoundY = ColumnIndex - 1 > 0 ? ColumnIndex - 1 : 0;
			int32 UpperBoundY = ColumnIndex + 1 < GridSize ? ColumnIndex + 1 : GridSize - 1;

			// Loop through neighbors
			for (int32 NeighborRowIndex = LowerBoundX; NeighborRowIndex <= UpperBoundX; NeighborRowIndex++)
			{
				for (int32 NeighborColumnIndex = LowerBoundY; NeighborColumnIndex <= UpperBoundY; NeighborColumnIndex++)
				{
					//Skip the calculation if this is the current tile
					if (NeighborRowIndex == RowIndex && NeighborColumnIndex == ColumnIndex) { continue; }
					// If the neighbor is populated, increment the neighbor count
					if (CurrentPopulatedStates[NeighborRowIndex][NeighborColumnIndex] == true)
					{
						NeighborCount++;
					}
				}
			}

			// Get current tile
			ASquare* CurrentSquare = Grid[RowIndex][ColumnIndex];

			// If the current tile is populated and has less than two or greater than 3 neighbors, it dies
			// Or if the current tile is not populated and has three neighbors, it becomes populated
			// Use the map of boolean states to calculate everything based on the current generation
			if ((CurrentPopulatedStates[RowIndex][ColumnIndex] == true && (NeighborCount < 2 || NeighborCount > 3))
			|| (CurrentPopulatedStates[RowIndex][ColumnIndex] == false && NeighborCount == 3))
			{
				CurrentSquare->TogglePopulated();
			}	
		}
	}
}

void AGrid::ResetGame()
{
	PauseGame();
	
	for (int32 RowIndex = 0; RowIndex < GridSize; RowIndex++)
	{
		for (int32 ColumnIndex = 0; ColumnIndex < GridSize; ColumnIndex++)
		{
			// Reset stored boolean states for population
			PopulatedStates[RowIndex][ColumnIndex] = false;

			// Reset tile states in grid
			ASquare* CurrentSquare = Grid[RowIndex][ColumnIndex];
			if (CurrentSquare->Populated)
			{
				CurrentSquare->TogglePopulated();
			}
		}
	}
}



