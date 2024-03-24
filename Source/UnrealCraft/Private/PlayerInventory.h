// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"

/**
 * For representing a players' inventory.
 */
class PlayerInventory : public Inventory
{
public:
	PlayerInventory();
	virtual ~PlayerInventory() override;

protected:
	virtual FIntVector2 GetDefaultSize() const override;
};
