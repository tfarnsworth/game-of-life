// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Square.generated.h"

//Forward declaration
class Material;
class MaterialInstance;
class StaticMeshComponent;
class AGrid;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEOFLIFE_API ASquare : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ASquare();

	// Toggles populated state and updates material accordingly
	UFUNCTION(BlueprintCallable, Category = Input)
	void TogglePopulated();

	// Stores coordinates of tile in grid
	void SetGridCoordinates(int32 RowToSet, int32 ColumnToSet);

	// Store a pointer to the parent grid
	void SetParentGrid(AGrid* GridToSet);

	bool Populated = false;


private:
	UStaticMeshComponent* SquareMeshComponent;
	UMaterial* BaseMaterial;
	UMaterialInstance* YellowMaterial;
	int32 Row;
	int32 Column;
	AGrid* Grid;
	
};
