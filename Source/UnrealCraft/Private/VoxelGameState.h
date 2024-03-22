// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryDatabase.h"
#include "GameFramework/GameStateBase.h"
#include "VoxelGameState.generated.h"

class UItemInfoDatabase;

/**
 * 
 */
UCLASS()
class AVoxelGameState : public AGameStateBase
{
	GENERATED_BODY()

	InventoryDatabase InventoryDatabase;

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemInfoDatabase> ItemInfoDatabase;
	
	::InventoryDatabase& GetInventoryDatabase()
	{
		return InventoryDatabase;
	}
};
