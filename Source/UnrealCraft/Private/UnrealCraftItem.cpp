// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCraftItem.h"

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
	this->AssociatedItemID = InAssociatedItemID;
	this->CurrentStack = CurrentStackSize;
}
