// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UUnrealCraftItem;
/**
 * For representing an {@link UUnrealCraftItem} in the GUI.
 */
UCLASS()
class UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UImage> IconImage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTextBlock> StackText;
	
	UPROPERTY()
	TObjectPtr<UUnrealCraftItem> RepresentedItem;

public:

	void SetRepresentedItem(TObjectPtr<UUnrealCraftItem> NewRepresentedItem);
	
};
