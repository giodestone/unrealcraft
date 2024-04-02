// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealCraftItemInfo.h"
#include "Engine/DataAsset.h"
#include "ItemDatabase.generated.h"

/**
 * Contains a read-only database of all items in the game, which can be added in editor.
 */
UCLASS()
class UItemInfoDatabase : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<FString, FUnrealCraftItemInfo> Items;

public:
	/**
	 * Get the array of items defined by the user.
	 */
	const TMap<FString, FUnrealCraftItemInfo>& GetItems() const;

	/**
	 * Get an item's info by its ID, as set in editor.
	 * @param ID The ID of the item.
	 * @return The item info associated with the ID.
	 * @remark If the item wasn't found, it returns the first item.
	 */
	FUnrealCraftItemInfo GetInfo(const FString& ID) const;

	/**
	 * Get an item's ID by block info.
	 * @param Block Block to search by.
	 * @return The first item info's ID associated with the block.
	 * @remark If the item wasn't found, the first item info's key is returned.
	 */
	FString GetInfo(const EBlock Block) const;

	
};
