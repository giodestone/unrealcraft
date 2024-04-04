// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"

UPlayerInventory::UPlayerInventory()
{
	SetSize(GetDefaultSize());

	HotbarSize = FIntVector2(GetSize().X, 1);
}

UPlayerInventory::~UPlayerInventory()
{
}

FIntVector2 UPlayerInventory::GetHotBarRowStartCoords() const
{
	return FIntVector2(0, GetSize().Y - 1);
}

const FIntVector2& UPlayerInventory::GetHotbarSize() const
{
	return HotbarSize;
}

int32 UPlayerInventory::GetCurrentlySelectedHotbarSlot() const
{
	return CurrentlySelectedHotbarSlot;
}

FIntVector2 UPlayerInventory::GetCurrentlySelectedHotbarSlotCoords() const
{
	auto StartCoords = GetHotBarRowStartCoords();
	StartCoords.X = GetCurrentlySelectedHotbarSlot();
	return StartCoords;
}

bool UPlayerInventory::SetCurrentlySelectedHotbarSlot(int32 NewlySelectedSlot)
{
	if (NewlySelectedSlot < 0 || NewlySelectedSlot > HotbarSize.X - 1)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[PlayerInventory::SetCurrentlySelectedHotbarSlot]: Param error: NewlySelectedSlot is negative or larger than hotbar size. Will not set new hotbar selection."));
		return false;
	}
	
	CurrentlySelectedHotbarSlot = NewlySelectedSlot;
	return true;
}

FIntVector2 UPlayerInventory::GetDefaultSize() const
{
	return FIntVector2(9, 5); // Last is for hotbar.
}
