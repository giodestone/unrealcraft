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
 * Logic for when the player is inside of the voxel world.
 */
UCLASS()
class AVoxelGameState : public AGameStateBase
{
	GENERATED_BODY()

	TUniquePtr<InventoryDatabase> MainInventoryDatabase;

	TSharedPtr<PlayerInventory> MainPlayerInventory;

	UPROPERTY()
	TObjectPtr<UInventoryVisualizerWidget> InventoryVisualizer;

public:
	/**
	 * Reference to the item info database.
	 */
	UPROPERTY(EditDefaultsOnly, Category="References")
	TObjectPtr<UItemInfoDatabase> ItemInfoDatabase;

	/**
	 * Reference to the inventory database that stores all inventories for the game.
	 */
	::InventoryDatabase& GetInventoryDatabase();

	/**
	 * Reference to the inventory visualiser, which visualises the player and any additional inventories in the world.
	 */
	TObjectPtr<UInventoryVisualizerWidget> GetInventoryVisualizer() const;

	/**
	 * Get the main player's inventory.
	 */
	TSharedPtr<PlayerInventory> GetPlayerInventory() const;

	/**
	 * Get a reference to the item info database which contains info about every items' icon, stack size etc.
	 */
	TObjectPtr<UItemInfoDatabase> GetItemInfoDatabase() const;

	/**
	 * Set the reference to inventory visualizer.
	 */
	void SetInventoryVisualizer(TObjectPtr<UInventoryVisualizerWidget> NewInventoryVisualizer);

	/**
	 * Set the reference to the main player's inventory.
	 */
	void SetPlayerInventory(TSharedPtr<PlayerInventory> NewPlayerInventory);

protected:
	virtual void BeginPlay() override;
};
