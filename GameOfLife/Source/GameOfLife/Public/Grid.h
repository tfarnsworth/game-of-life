// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

//Forward declarations
class ASquare;

UCLASS()
class GAMEOFLIFE_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();
	void UpdatePopulatedState(int32 Row, int32 Column, bool Populated);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	const int32 GridSize = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ASquare> Square;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float GameTickSpeed = 5.f;

	//TODO initialize arrays using grid size
	ASquare* Grid[20][20];
	bool PopulatedStates[20][20];
	void InitializeGrid();
	
	UFUNCTION(BlueprintCallable, Category = "Game Timer")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "Game Timer")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Game Timer")
	void ResetGame();

	FTimerHandle GameTimer;

	UFUNCTION()
	void AdvanceGameState();
};
