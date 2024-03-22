// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "InventoryVisualizerWidget.h"

/**
 * Generic inventory which stores some amount of items.
 */
class Inventory : public IInventoryInterface
{
	const FIntVector2 DefaultSize = FIntVector2(9, 4);
	
	TMap<FIntVector2, TObjectPtr<UUnrealCraftItem>> Items;
	FIntVector2 InventorySize;
	
public:
	Inventory();
	virtual ~Inventory() override;
	virtual bool InsertAnywhere(UUnrealCraftItem* Item) override;
	virtual bool InsertInto(const FIntVector2& Coord, UUnrealCraftItem* Item) override;
	virtual bool RemoveFrom(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) override;
	virtual const FIntVector2 GetSize() const override;
	virtual bool HasItem(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) const override;

protected:
	virtual void SetSize(FIntVector2 Size) override;

	/**
	 * Check whether the coord is between 0 and one less than Size.X, and Size.Y.
	 */
	bool IsCoordInBounds(const FIntVector2& Coord) const;
	
};
