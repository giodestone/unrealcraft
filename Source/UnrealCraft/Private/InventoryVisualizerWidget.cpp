// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryVisualizerWidget.h"

#include "InventoryInterface.h"
#include "Blueprint/WidgetTree.h"

void UInventoryVisualizerWidget::ShowPlayerInventory(TSharedPtr<IInventoryInterface> PlayerInventory)
{
	for (int32 x = 0; x < PlayerInventory->GetSize().X; x++)
	{
		for (int32 y = 0; y < PlayerInventory->GetSize().Y; y++)
		{
			CreateWidget<UInventorySlotWidget>(PlayerInventorySlotParent);
		}
	}
	// spawn item slots
	// place items in slots
	// show the player inventory

	this->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryVisualizerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> Children;
	WidgetTree->GetAllWidgets(Children);
	
	if (auto PlayerWidget = GetWidgetFromName(PlayerInventorySlotWidgetName))
	{
		PlayerInventorySlotWidget = Cast<UUserWidget>(PlayerWidget);
	}
	else
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Unable to get player widget."));
	}
	
	if (PlayerInventorySlotWidget != nullptr)
	{
		if (auto SlotParent = PlayerInventorySlotWidget->GetWidgetFromName(FName("InventorySlotParent")))
		{
			this->PlayerInventorySlotParent = SlotParent;
		}
		else
		{
			GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Unable to get inventory slot parent for player inventory."));
		}
	}
	else
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Player inventory widget not set."));
	}

	this->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryVisualizerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
