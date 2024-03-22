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

	UPROPERTY(EditDefaultsOnly)
	FName IconImageWidgetName;

	UPROPERTY(EditDefaultsOnly)
	FName StackTextWidgetName;
	
	UPROPERTY()
	TObjectPtr<class UImage> IconImage;

	UPROPERTY()
	TObjectPtr<class UTextBlock> StackText;
	
	UPROPERTY()
	TObjectPtr<UUnrealCraftItem> RepresentedItem;

public:

	void SetRepresentedItem(TObjectPtr<UUnrealCraftItem> NewRepresentedItem);

protected:
	virtual void NativeOnInitialized() override;
};
