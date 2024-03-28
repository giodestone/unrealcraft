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
	 * Get an item's info by its ID, as set in editor.
	 * @param ID The ID of the item.
	 * @return The item info associated with the ID.
	 * @remark If the item wasn't found, it returns the first item.
	 */
	FUnrealCraftItemInfo GetInfo(const FString& ID) const;
};
