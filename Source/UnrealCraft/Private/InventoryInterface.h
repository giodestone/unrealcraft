// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UUnrealCraftItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * For interacting with inventories.
 */
class IInventoryInterface
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual void SetSize(FIntVector2 Size) = 0;
	
public:
	UFUNCTION()
	virtual bool InsertAnywhere(UUnrealCraftItem* Item) = 0;
	
	UFUNCTION()
	virtual bool InsertInto(const FIntVector2& Coord, UUnrealCraftItem* Item) = 0;

	UFUNCTION()
	virtual bool RemoveFrom(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) = 0;

	UFUNCTION()
	virtual bool HasItem(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) const = 0;

	UFUNCTION()
	virtual const FIntVector2 GetSize() const = 0;	
};
