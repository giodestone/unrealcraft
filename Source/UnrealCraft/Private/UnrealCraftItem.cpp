// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCraftItem.h"

UUnrealCraftItem::UUnrealCraftItem()
{
}

UUnrealCraftItem::~UUnrealCraftItem()
{
}

void UUnrealCraftItem::Initialize(UUnrealCraftItemInfo* ItemInfo, const int8 StackSize)
{
	this->RepresentedItemInfo = ItemInfo;
	this->CurrentStack = StackSize;
}
