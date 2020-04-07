// Fill out your copyright notice in the Description page of Project Settings.


#include "Square.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Grid.h"

ASquare::ASquare()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> SquareMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> YellowMaterial;
		FConstructorStatics()
			: SquareMesh(TEXT("/Game/Meshes/Tile.Tile"))
			, BaseMaterial(TEXT("/Game/Materials/BaseMaterial.BaseMaterial"))
			, YellowMaterial(TEXT("/Game/Materials/YellowMaterial.YellowMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	YellowMaterial = ConstructorStatics.YellowMaterial.Get();

	SquareMeshComponent = GetStaticMeshComponent();
	SquareMeshComponent->SetStaticMesh(ConstructorStatics.SquareMesh.Get());
	SquareMeshComponent->SetMaterial(0, BaseMaterial);
	SquareMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

// Set populated state, notify parent grid of the change, and update the material to visually represent state
void ASquare::TogglePopulated()
{
	UE_LOG(LogTemp, Warning, TEXT("Toggling populated"));
	Populated = !Populated;
	Grid->UpdatePopulatedState(Row, Column, Populated);
	if (Populated)
	{
		SquareMeshComponent->SetMaterial(0, YellowMaterial);
	}
	else
	{
		SquareMeshComponent->SetMaterial(0, BaseMaterial);
	}
}

// Stores this square's location in the grid
void ASquare::SetGridCoordinates(int32 RowToSet, int32 ColumnToSet)
{
	Row = RowToSet;
	Column = ColumnToSet;
}

// Stores a reference to the parent grid
void ASquare::SetParentGrid(AGrid* GridToSet)
{
	Grid = GridToSet;
}

