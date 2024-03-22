// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"

/**
 * Contains information about the various inventories present in the game world at runtime.
 */
class InventoryDatabase
{
	// This can be rolled into one, but it makes it way easier to debug.
	
	TMap<FIntVector, TSharedPtr<IInventoryInterface>> WorldInventories;
	TMap<FString, TSharedPtr<IInventoryInterface>> EntityInventories;
	
public:
	InventoryDatabase();
	~InventoryDatabase();

	/**
	 * Setup/rest all inventories.
	 */
	void Initialize();

	void AddWorldInventory(const FIntVector Coord, const TSharedPtr<IInventoryInterface> Inventory);
	
	void AddEntityInventory(const FString ID, const TSharedPtr<IInventoryInterface> Inventory);
	
	/**
	 * Get an inventory present in the block world.
	 * @return True if it exists, false if it doesn't.
	 */
	bool GetWorldInventory(const FIntVector& Coord, TSharedPtr<IInventoryInterface>& OutInventory) const;

	/**
	 * Get an inventory outside the block world.
	 * @return True if it exists, false if it doesn't.
	 * @remark For ID use "Player" if getting the player inventory.
	 */
	bool GetEntityInventory(const FString& ID, TSharedPtr<IInventoryInterface>& OutInventory) const;

	bool DoesEntityInventoryExist(const FString& ID) const;
	
};
