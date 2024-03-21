// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCraft/Public/Inventory.h"

#include "UnrealCraft/Public/InventoryInterface.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

bool Inventory::Open()
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString("Inventory opened!!!!!!!!"));
	return false;
}

bool Inventory::Close()
{
	return false;
}

bool Inventory::InsertAnywhere(UUnrealCraftItem* Item)
{
	return false;
}

bool Inventory::InsertInto(const FIntVector2& Coord, UUnrealCraftItem* Item)
{
	return false;
}

bool Inventory::RemoveFrom(const FIntVector2& Coord, UUnrealCraftItem*& OutItem)
{
	return false;
}

bool Inventory::IsFull() const
{
	return false;
}

const FIntVector2 Inventory::GetSize() const
{
	return FIntVector2(0,0);
}
