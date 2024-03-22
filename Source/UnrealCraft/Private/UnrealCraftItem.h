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

public:
	FString GetAssociatedItemID() const;
	int8 GetCurrentStack() const;

private:
	UPROPERTY(VisibleAnywhere)
	int8 CurrentStack;
public:
	UUnrealCraftItem();
	~UUnrealCraftItem();

	void Initialize(const FString& InAssociatedItemID, const int8 CurrentStackSize = 1);
};
