// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "CoreMinimal.h"
#include "InventoryVisualizerWidget.h"
#include "VoxelGameState.h"
#include "InventoryInterface.h"

Inventory::Inventory()
{
	SetSize(DefaultSize); // Yes call any derived class.
}

Inventory::~Inventory()
{
}

bool Inventory::InsertAnywhere(UUnrealCraftItem* Item)
{
	// find first empty slot
	FIntVector2 PreferedSlot(0,0);
	
	for (auto ItemTuple : Items)
	{
		if (ItemTuple.Key == PreferedSlot)
		{
			// TODO: implement stacking
			if (PreferedSlot.X < InventorySize.X)
				PreferedSlot.X += 1;
			else if (PreferedSlot.Y + 1 >= InventorySize.Y)
				return false;
			else if (PreferedSlot.X >= InventorySize.X)
			{
				PreferedSlot.X = 0;
				PreferedSlot.Y++;
			}
			else
			{
				GLog->Log(ELogVerbosity::Error, TEXT("[Inventory::InsertAnywhere]: The programmer didn't think this case would be reached."));
				return false;
			}
		}
		else
			break;
	}

	return InsertInto(PreferedSlot, Item);
}

bool Inventory::InsertInto(const FIntVector2& Coord, UUnrealCraftItem* Item)
{
	if (!IsCoordInBounds(Coord))
		return false;

	if (Items.Contains(Coord))
		return false;

	// TODO: implement stacking
	
	Items.Add(Coord, Item);

	return true;
}

bool Inventory::RemoveFrom(const FIntVector2& Coord, UUnrealCraftItem*& OutItem)
{
	// TODO: Consider stacking.
	
	TObjectPtr<UUnrealCraftItem> RemovedItem;
	auto ReturnValue = Items.RemoveAndCopyValue(Coord, RemovedItem);
	OutItem = RemovedItem;
	return ReturnValue;
}

const FIntVector2 Inventory::GetSize() const
{
	return InventorySize;
}

bool Inventory::HasItem(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) const
{
	if (!IsCoordInBounds(Coord))
		return false;

	if (!Items.Contains(Coord))
		return false;

	OutItem = Items[Coord];
	return true;
}

void Inventory::SetSize(FIntVector2 Size)
{
	for (auto ItemTuple : Items)
	{
		if (ItemTuple.Key.X > Size.X || ItemTuple.Key.Y > Size.Y)
		{
			// TODO: Support inventory size decrease.
			GLog->Log(ELogVerbosity::Error, TEXT("[Inventory::SetSize]: Decrease of inventory size is currently unsupported."));
			break;
		}
	}
	
	this->InventorySize = Size;
}

bool Inventory::IsCoordInBounds(const FIntVector2& Coord) const
{
	return Coord.X < InventorySize.X && Coord.Y < InventorySize.Y && Coord.X >= 0 && Coord.Y >= 0;
}
