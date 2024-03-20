// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryDatabase.h"

InventoryDatabase::InventoryDatabase()
{
}

InventoryDatabase::~InventoryDatabase()
{
}

void InventoryDatabase::Initialize()
{
	EntityInventories.Empty();
	WorldInventories.Empty();
}

void InventoryDatabase::AddWorldInventory(const FIntVector Coord, const TSharedPtr<IInventoryInterface> Inventory)
{
	WorldInventories.Add(Coord, Inventory);
}

void InventoryDatabase::AddEntityInventory(const FString ID, const TSharedPtr<IInventoryInterface> Inventory)
{
	EntityInventories.Add(ID, Inventory);
}

bool InventoryDatabase::GetWorldInventory(const FIntVector& Coord, TSharedPtr<IInventoryInterface>& OutInventory) const
{
	if (!WorldInventories.Contains(Coord))
		return false;

	OutInventory = WorldInventories[Coord];

	return true;
}

bool InventoryDatabase::GetEntityInventory(const FString& ID, TSharedPtr<IInventoryInterface>& OutInventory) const
{
		if (!EntityInventories.Contains(ID))
    		return false;
    
    	OutInventory = EntityInventories[ID];
    
    	return true;
}

bool InventoryDatabase::DoesEntityInventoryExist(const FString& ID) const
{
	return EntityInventories.Contains(ID);
}

