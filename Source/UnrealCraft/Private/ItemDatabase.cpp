// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDatabase.h"

FUnrealCraftItemInfo UItemInfoDatabase::GetInfo(const FString& ID) const
{
	if (!Items.Contains(ID))
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UItemInfoDatabase::GetInfo]: Item not found."));
		return Items.Array()[0].Value;
	}

	return Items[ID];
}
