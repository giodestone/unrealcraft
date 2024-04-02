// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "InventoryVisualizerWidget.h"

/**
 * Generic inventory which stores some amount of items in memory.
 */
class Inventory : public IInventoryInterface
{
	UPROPERTY()
	TMap<FIntVector2, TObjectPtr<UUnrealCraftItem>> Items;
	
	FIntVector2 InventorySize;

public:
	Inventory();
	virtual ~Inventory() override;
	virtual bool InsertAnywhereStacked(UUnrealCraftItem* Item) override;
	virtual bool InsertInto(const FIntVector2& Coord, UUnrealCraftItem* Item) override;
	virtual bool RemoveFrom(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) override;
	virtual const FIntVector2 GetSize() const override;
	virtual bool HasItem(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) const override;
	virtual bool RemoveNumberFrom(const FIntVector2& Coord, const int8 NumToRemove, UUnrealCraftItem*& OutItem) override;

protected:
	virtual void SetSize(FIntVector2 Size) override;

	/**
	 * Get the default size of the inventory.
	 * @remarks The retrun value of this function should be modified in derived classes if the default size should change.
	 */
	virtual FIntVector2 GetDefaultSize() const;
	
	/**
	 * Check whether the coord is between 0 and one less than Size.X, and Size.Y.
	 */
	bool IsCoordInBounds(const FIntVector2& Coord) const;

	bool GetCoordOfNonFullStack(const UUnrealCraftItem* Item, FIntVector2& OutCoord) const;

	bool GetFirstEmptyCoord(FIntVector2& OutCoord) const;

public:
	
};
