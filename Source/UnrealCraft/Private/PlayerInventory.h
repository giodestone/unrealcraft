// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"

/**
 * For representing a players' inventory.
 */
class PlayerInventory : public Inventory
{
	FIntVector2 HotbarSize;
	
public:
	PlayerInventory();
	virtual ~PlayerInventory() override;

	/**
	 * The beginning coordinate of the inventory of the row that represents the hotbar.
	 */
	virtual FIntVector2 GetHotBarRowStartCoords() const;

	/**
	 * Get the current size of the hotbar.
	 */
	const FIntVector2& GetHotbarSize() const;
	
protected:
	virtual FIntVector2 GetDefaultSize() const override;
};
