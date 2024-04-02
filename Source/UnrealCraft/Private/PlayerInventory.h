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

	int32 CurrentlySelectedHotbarSlot = 0;
	
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

	/**
	 * Which slot is currently selected by the player.
	 */
	int32 GetCurrentlySelectedHotbarSlot() const;

	/**
	 * Get an inventory index for the currently selected hotbar slot.
	 */
	FIntVector2 GetCurrentlySelectedHotbarSlotCoords() const;

	/**
	 * Set the newly selected slot by the player.
	 * @returns @code true@endcode if set successfully, @code false@endcode if NewlySelectedSlot is out of bounds (negative or more than @code HotbarSize.X - 1@endcode).
	 * @remark This only sets it inventory side. It does not notify any visualizers.
	 */
	bool SetCurrentlySelectedHotbarSlot(int32 NewlySelectedSlot);
	
protected:
	virtual FIntVector2 GetDefaultSize() const override;
};
