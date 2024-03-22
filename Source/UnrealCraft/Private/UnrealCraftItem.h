// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealCraftItemInfo.h"
#include "UnrealCraftItem.generated.h"

/**
 * An item present in some inventory or dropped item.
 */
UCLASS()
class UUnrealCraftItem : public UObject
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FString AssociatedItemID;

	UPROPERTY(VisibleAnywhere)
	int8 CurrentStack;
public:
	UUnrealCraftItem();
	~UUnrealCraftItem();

	void Initialize(const FString& InAssociatedItemID, const int8 CurrentStackSize = 1);
};
