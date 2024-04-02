// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UUnrealCraftItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Generic interface for interacting with inventories.
 */
class IInventoryInterface
{
	GENERATED_BODY()

protected:
	/**
	 * Set the new size of the inventory.
	 * @param Size The new size.
	 */
	UFUNCTION()
	virtual void SetSize(FIntVector2 Size) = 0;
	
public:
	/**
	 * Get the current size of the inventory.
	 */
	UFUNCTION()
	virtual const FIntVector2 GetSize() const = 0;	

	/**
	 * Insert Item into anywhere in the inventory, attempting to stack with existing ones if possible.
	 * @param Item The item to insert. The passed in pointer must not be used anymore as it may not refer to the item in the inventory.
	 * @return @code true@endcode if item was inserted, @code false@endcode if not.
	 * @remark If doing anything with the Item afterwards, a new reference to it must be retrived, as the Item pointer may be invalid as it may no longer point to one in the inventory.
	 */
	UFUNCTION()
	virtual bool InsertAnywhereStacked(UUnrealCraftItem* Item) = 0;

	/**
	 * Insert an item into a specific coordiante that must be empty.
	 * @param Coord The coordinate to try to insert the item to.
	 * @param Item The item to insert.
	 * @return @code true@endcode if item was inserted, @code false@endcode if not.
	 */
	UFUNCTION()
	virtual bool InsertInto(const FIntVector2& Coord, UUnrealCraftItem* Item) = 0;

	/**
	 * Remove an item from a specific coordinate, if possible.
	 * @param Coord Which coordinate to remove from
	 * @param OutItem The item that was inside of this coordinate. @code nullptr@endcode if nothing was there. It is removed from the inventory.
	 * @return @code true@endcode if the item was removed, @code false@endcode if not.
	 */
	UFUNCTION()
	virtual bool RemoveFrom(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) = 0;

	/**
	 * Removes a certain amount of items from a coordiante, if possible.
	 * @param Coord Coordinate to remove from.
	 * @param NumToRemove Number to remove.
	 * @param OutItem Either the newly created item with a stack of NumToRemove; or the items that were already there.
	 * @return @code true@endcode if it is possible to remove at least NumToRemove. @code false@endcode if not.
	 * @remark Will create a new item if the stack size is larger than provided. Otherwise, the whole stack is removed.
	 * @remark Visualisers must be activated manually.
	 */
	UFUNCTION()
	virtual bool RemoveNumberFrom(const FIntVector2& Coord, const int8 NumToRemove, UUnrealCraftItem*& OutItem) = 0;

	/**
	 * Check whether the inventory has the item, and if so returns a reference to the item.
	 * @remarks Does not remove the item from the inventory.
	 * @param Coord The coordinate to check
	 * @param OutItem The item at that coordinate. It remains in the inventory.
	 * @return @code true@endcode if item is present, @code false@endcode if not.
	 */
	UFUNCTION()
	virtual bool HasItem(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) const = 0;
};
