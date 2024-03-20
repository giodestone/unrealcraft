// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"

/**
 * For representing a players' inventory.
 */
class UNREALCRAFT_API PlayerInventory : public Inventory
{
public:
	PlayerInventory();
	~PlayerInventory();
};
