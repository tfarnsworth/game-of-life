// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "ClickableTile.h"
#include "Grid.h"

UTile::UTile()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> TileMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> YellowMaterial;
		FConstructorStatics()
			: TileMesh(TEXT("/Game/Meshes/Tile.Tile"))
			, BaseMaterial(TEXT("/Game/Materials/BaseMaterial.BaseMaterial"))
			, YellowMaterial(TEXT("/Game/Materials/YellowMaterial.YellowMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	YellowMaterial = ConstructorStatics.YellowMaterial.Get();

	SetStaticMesh(ConstructorStatics.TileMesh.Get());
	SetMaterial(0, BaseMaterial);
	SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void UTile::TogglePopulated()
{
	Populated = !Populated;
	Grid->UpdatePopulatedState(Row, Column, Populated);
	if (Populated)
	{
		SetMaterial(0, YellowMaterial);
	}
	else
	{
		SetMaterial(0, BaseMaterial);
	}
}

void UTile::SetGridCoordinates(int32 RowToSet, int32 ColumnToSet)
{
	Row = RowToSet;
	Column = ColumnToSet;
}

void UTile::SetParentGrid(AGrid* GridToSet)
{
	Grid = GridToSet;
}

