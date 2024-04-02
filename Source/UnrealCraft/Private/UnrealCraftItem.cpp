// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCraftItem.h"

#include "ItemDatabase.h"

FString UUnrealCraftItem::GetAssociatedItemID() const
{
	return AssociatedItemID;
}

int8 UUnrealCraftItem::GetCurrentStack() const
{
	return CurrentStack;
}

UUnrealCraftItem::UUnrealCraftItem()
{
}

UUnrealCraftItem::~UUnrealCraftItem()
{
}

void UUnrealCraftItem::Initialize(const FString& InAssociatedItemID, const int8 CurrentStackSize)
{
	this->AssociatedItemID = FString(InAssociatedItemID);
	this->CurrentStack = CurrentStackSize;
}

bool UUnrealCraftItem::SetCurrentStackUnchecked(int8 NewStack)
{
	if (NewStack == 0)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UUnrealCraftItem::SetCurrentStackUnchecked]: Cannot set stack value to 0. This item should be removed from its parent inventory and destoryed instead."));
		return false;
	}

	if (NewStack <= 0)
		return false;

	CurrentStack = NewStack;

	return true;
}

bool UUnrealCraftItem::SetCurrentStackChecked(int8 NewStack, const UItemInfoDatabase* ItemInfoDB)
{
	if (ItemInfoDB == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UUnrealCraftItem::SetCurrentStackChecked]: Param error: ItemInfoDB null. Will not change stack size."));

		return false;
	}

	// To help debugging.
	if (NewStack == 0)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UUnrealCraftItem::SetCurrentStackChecked]: Cannot set stack value to 0. This item should be removed from its parent inventory and destoryed instead."));
		return false;
	}

	if (NewStack <= 0)
		return false;

	if (NewStack > ItemInfoDB->GetInfo(GetAssociatedItemID()).MaxStackSize)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UUnrealCraftItem::SetCurrentStackChecked]: Param error: New stack size exceeds represented item's max stack size. Will not set new stack size."));
		return false;
	}

	CurrentStack = NewStack;

	return true;
}
