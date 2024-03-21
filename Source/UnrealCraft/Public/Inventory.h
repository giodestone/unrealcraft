// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"

/**
 * Generic inventory which stores some amount of items.
 */
class UNREALCRAFT_API Inventory : public IInventoryInterface
{
public:
	Inventory();
	virtual ~Inventory() override;
	virtual bool Open() override;
	virtual bool Close() override;
	virtual bool InsertAnywhere(UUnrealCraftItem* Item) override;
	virtual bool InsertInto(const FIntVector2& Coord, UUnrealCraftItem* Item) override;
	virtual bool RemoveFrom(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) override;
	virtual bool IsFull() const override;
	virtual const FIntVector2 GetSize() const override;
};
