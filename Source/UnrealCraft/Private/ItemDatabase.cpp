// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDatabase.h"

const TMap<FString, FUnrealCraftItemInfo>& UItemInfoDatabase::GetItems() const
{
	return Items;
}

FUnrealCraftItemInfo UItemInfoDatabase::GetInfo(const FString& ID) const
{
	if (!Items.Contains(ID))
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UItemInfoDatabase::GetInfo]: Item not found."));
		return Items.Array()[0].Value;
	}

	return Items[ID];
}

FString UItemInfoDatabase::GetInfo(const EBlock Block) const
{
	for (TTuple<FString, FUnrealCraftItemInfo> Item : Items)
	{
		if (Item.Value.BlockType == Block)
			return Item.Key;
	}

	return Items.Array()[0].Key;
}
