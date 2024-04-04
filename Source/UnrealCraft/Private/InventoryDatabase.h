// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "InventoryDatabase.generated.h"

/**
 * Contains information about the various inventories present in the game world at runtime.
 */
UCLASS()
class UInventoryDatabase : public UObject
{
	GENERATED_BODY()
	
	// These variables can be rolled into one, but it makes it way easier to debug.
	UPROPERTY()
	TMap<FIntVector, TScriptInterface<IInventoryInterface>> WorldInventories;

	UPROPERTY()
	TMap<FString, TScriptInterface<IInventoryInterface>> EntityInventories;
	
public:
	UInventoryDatabase();
	virtual ~UInventoryDatabase() override;

	/**
	 * Setup/reset all inventories.
	 */
	void Initialize();

	/**
	 * Add a new inventory which is present at a specfic world coordinate.
	 * @param Coord Where the inventory is located in chunk coordinates.
	 * @param Inventory The new inventory to add.
	 */
	void AddWorldInventory(const FIntVector Coord, const TScriptInterface<IInventoryInterface> Inventory);

	/**
	 * Add a new entity inventory that is present in the game world.
	 * @remark Inclues new player inventories.
	 * @param ID The ID of the inventory. This will be used to retrieve it later.
	 * @param Inventory The new inventory.
	 */
	void AddEntityInventory(const FString ID, const TScriptInterface<IInventoryInterface> Inventory);
	
	/**
	 * Get an inventory present in the block world.
	 * @return True if it exists, false if it doesn't.
	 */
	bool GetWorldInventory(const FIntVector& Coord, TScriptInterface<IInventoryInterface>& OutInventory) const;

	/**
	 * Get an inventory outside the block world.
	 * @return True if it exists, false if it doesn't.
	 * @remark For ID use "Player" if getting the player inventory.
	 */
	bool GetEntityInventory(const FString& ID, TScriptInterface<IInventoryInterface>& OutInventory) const;

	/**
	 * Whether an entity nventory with a specific ID exists.
	 * @param ID The ID of the inventory to check.
	 * @return @code true@endcode if the inventory exists, @code false@endcode if not.
	 */
	bool DoesEntityInventoryExist(const FString& ID) const;
	
};
