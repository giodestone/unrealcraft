// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryDatabase.h"
#include "GameFramework/GameStateBase.h"
#include "VoxelGameState.generated.h"

/**
 * 
 */
UCLASS()
class AVoxelGameState : public AGameStateBase
{
	GENERATED_BODY()

	InventoryDatabase InventoryDatabase;

public:
	::InventoryDatabase& GetInventoryDatabase()
	{
		return InventoryDatabase;
	}
};
