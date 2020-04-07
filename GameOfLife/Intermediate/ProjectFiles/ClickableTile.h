// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClickableTile.generated.h"

UCLASS()
class GAMEOFLIFE_API AClickableTile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClickableTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
