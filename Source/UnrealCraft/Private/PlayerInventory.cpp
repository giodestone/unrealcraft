// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"

PlayerInventory::PlayerInventory()
{
	SetSize(GetDefaultSize());
}

PlayerInventory::~PlayerInventory()
{
}

FIntVector2 PlayerInventory::GetHotBarRowStartCoords() const
{
	return FIntVector2(0, GetSize().Y - 1);
}

FIntVector2 PlayerInventory::GetDefaultSize() const
{
	return FIntVector2(9, 5); // Last is for hotbar.
}
