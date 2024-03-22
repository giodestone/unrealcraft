// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDatabase.h"

FUnrealCraftItemInfo UItemInfoDatabase::GetInfo(const FString& ID) const
{
	return Items[ID];
}
