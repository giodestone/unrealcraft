// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryDatabase.h"
#include "GameFramework/GameStateBase.h"
#include "InventoryVisualizerWidget.h"
#include "VoxelGameState.generated.h"

class PlayerInventory;
class UItemInfoDatabase;

/**
 * 
 */
UCLASS()
class AVoxelGameState : public AGameStateBase
{
	GENERATED_BODY()

	InventoryDatabase InventoryDatabase;

	UPROPERTY()
	TObjectPtr<UInventoryVisualizerWidget> InventoryVisualizer;

	TSharedPtr<PlayerInventory> MainPlayerInventory;

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemInfoDatabase> ItemInfoDatabase;

	
	::InventoryDatabase& GetInventoryDatabase() { return InventoryDatabase; }

	TObjectPtr<UInventoryVisualizerWidget> GetInventoryVisualizer() const { return InventoryVisualizer; }
	
	void SetInventoryVisualizer(TObjectPtr<UInventoryVisualizerWidget> NewInventoryVisualizer) { InventoryVisualizer = NewInventoryVisualizer; }

	TSharedPtr<PlayerInventory> GetPlayerInventory() const { return MainPlayerInventory; }
	
	void SetPlayerInventory(TSharedPtr<PlayerInventory> NewPlayerInventory) { MainPlayerInventory = NewPlayerInventory; }

	TObjectPtr<UItemInfoDatabase> GetItemInfoDatabase() const { return ItemInfoDatabase; }
};
