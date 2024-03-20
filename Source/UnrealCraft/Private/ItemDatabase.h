// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealCraftItemInfo.h"
#include "Engine/DataAsset.h"
#include "ItemDatabase.generated.h"

/**
 * Contains an editor-friendly way of adding a read-only database of all items in the game.
 */
UCLASS()
class UItemDatabase : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FUnrealCraftItemInfo> Items;
};
