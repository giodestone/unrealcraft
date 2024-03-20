// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * For interacting with inventories.
 */
class UNREALCRAFT_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual bool Open() = 0;

	UFUNCTION()
	virtual bool Close() = 0;
	
	UFUNCTION()
	virtual bool InsertAnywhere() = 0;
	
	UFUNCTION()
	virtual bool InsertInto(const FIntVector2& Coord/*, TSharedPtr<Stack> InItemStack*/) = 0;

	UFUNCTION()
	virtual bool RemoveFrom(const FIntVector2& Coord/*, TSharedPtr<Stack> OutItemStack*/) = 0;
	
	UFUNCTION()
	virtual bool IsFull() const = 0;

	UFUNCTION()
	virtual const FIntVector2 GetSize() const = 0;	
};
