// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealCraftItemInfo.h"
#include "UnrealCraftItem.generated.h"

/**
 * An item present in some inventory or dropped item.
 * @remarks Only one object should hold this item when not in transit. Otherwise, the same item will be duplicated across multiple inventories or items.
 */
UCLASS()
class UUnrealCraftItem : public UObject
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FString AssociatedItemID;

	UPROPERTY(VisibleAnywhere)
	int8 CurrentStack;

public:
	/**
	 * The item ID, as inside of the UItemInfoDatabase
	 */
	FString GetAssociatedItemID() const;

	/**
	 * How many of the current item are stacked.
	 * @remark Note datatype.
	 */
	int8 GetCurrentStack() const;


	UUnrealCraftItem();
	virtual ~UUnrealCraftItem() override;

	/**
	 * Initialize the item when created. This must be called.
	 * @param InAssociatedItemID The ID of the item it represents inside of the UItemInfoDatabase
	 * @param CurrentStackSize The stack size to start with.
	 */
	void Initialize(const FString& InAssociatedItemID, const int8 CurrentStackSize = 1);
};
