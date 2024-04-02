// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"

PlayerInventory::PlayerInventory()
{
	SetSize(GetDefaultSize());

	HotbarSize = FIntVector2(GetSize().X, 1);
}

PlayerInventory::~PlayerInventory()
{
}

FIntVector2 PlayerInventory::GetHotBarRowStartCoords() const
{
	return FIntVector2(0, GetSize().Y - 1);
}

const FIntVector2& PlayerInventory::GetHotbarSize() const
{
	return HotbarSize;
}

int32 PlayerInventory::GetCurrentlySelectedHotbarSlot() const
{
	return CurrentlySelectedHotbarSlot;
}

FIntVector2 PlayerInventory::GetCurrentlySelectedHotbarSlotCoords() const
{
	auto StartCoords = GetHotBarRowStartCoords();
	StartCoords.X = GetCurrentlySelectedHotbarSlot();
	return StartCoords;
}

bool PlayerInventory::SetCurrentlySelectedHotbarSlot(int32 NewlySelectedSlot)
{
	if (NewlySelectedSlot < 0 || NewlySelectedSlot > HotbarSize.X - 1)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[PlayerInventory::SetCurrentlySelectedHotbarSlot]: Param error: NewlySelectedSlot is negative or larger than hotbar size. Will not set new hotbar selection."));
		return false;
	}
	
	CurrentlySelectedHotbarSlot = NewlySelectedSlot;
	return true;
}

FIntVector2 PlayerInventory::GetDefaultSize() const
{
	return FIntVector2(9, 5); // Last is for hotbar.
}
