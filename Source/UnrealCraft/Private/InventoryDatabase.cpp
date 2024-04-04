// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryDatabase.h"

UInventoryDatabase::UInventoryDatabase()
{
}

UInventoryDatabase::~UInventoryDatabase()
{
}

void UInventoryDatabase::Initialize()
{
	EntityInventories.Empty();
	WorldInventories.Empty();
}

void UInventoryDatabase::AddWorldInventory(const FIntVector Coord, const TScriptInterface<IInventoryInterface> Inventory)
{
	WorldInventories.Add(Coord, Inventory);
}

void UInventoryDatabase::AddEntityInventory(const FString ID, const TScriptInterface<IInventoryInterface> Inventory)
{
	EntityInventories.Add(ID, Inventory);
}

bool UInventoryDatabase::GetWorldInventory(const FIntVector& Coord, TScriptInterface<IInventoryInterface>& OutInventory) const
{
	if (!WorldInventories.Contains(Coord))
		return false;

	OutInventory = WorldInventories[Coord];

	return true;
}

bool UInventoryDatabase::GetEntityInventory(const FString& ID, TScriptInterface<IInventoryInterface>& OutInventory) const
{
		if (!EntityInventories.Contains(ID))
    		return false;
    
    	OutInventory = EntityInventories[ID];
    
    	return true;
}

bool UInventoryDatabase::DoesEntityInventoryExist(const FString& ID) const
{
	return EntityInventories.Contains(ID);
}

