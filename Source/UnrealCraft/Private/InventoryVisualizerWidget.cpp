// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryVisualizerWidget.h"

#include "InventoryInterface.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "VoxelGameState.h"
#include "Blueprint/WidgetTree.h"

void UInventoryVisualizerWidget::TogglePlayerInventory(TSharedPtr<IInventoryInterface> PlayerInventory, bool& OutIsMenuDisplayed)
{
	switch (State)
	{
	case Hidden:
		InitPlayerInventoryWidget(PlayerInventory);
		State = ShowingPlayer;
		OutIsMenuDisplayed = true;
		break;
	case ShowingBoth:
		HideSecondaryInventory();
	case ShowingPlayer:
		State = Hidden;
		HidePlayerInventory();
		OutIsMenuDisplayed = false;
		break;
		
	}
	// TArray<UWidget*> Children;
	// PlayerInventoryMenuWidget->WidgetTree->GetAllWidgets(Children);
}

void UInventoryVisualizerWidget::ShowBothInventories(IInventoryInterface* PlayerInventory,
	IInventoryInterface* OtherInventory)
{
}

void UInventoryVisualizerWidget::Hide()
{
}

void UInventoryVisualizerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (auto PlayerWidget = GetWidgetFromName(PlayerInventoryMenuWidgetName))
	{
		PlayerInventoryMenuWidget = Cast<UUserWidget>(PlayerWidget);
	}
	else
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Unable to get player widget."));
	}
	
	if (PlayerInventoryMenuWidget != nullptr)
	{
		if (auto SlotParent = PlayerInventoryMenuWidget->GetWidgetFromName(FName("InventorySlotParent")))
		{
			if (auto SlotParentPanel = Cast<UPanelWidget>(SlotParent))
			{
				this->PlayerInventoryMenuWidgetSlotParent = SlotParentPanel;
			}
			else
				GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Unable to cast slot parent widget to a panel widget."));

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

	Cast<AVoxelGameState>(GetWorld()->GetGameState())->SetInventoryVisualizer(this);
	
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryVisualizerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryVisualizerWidget::InitPlayerInventoryWidget(TSharedPtr<IInventoryInterface> PlayerInventory)
{
	for (int32 x = 0; x < PlayerInventory->GetSize().X; x++)
	{
		for (int32 y = 0; y < PlayerInventory->GetSize().Y; y++)
		{
			auto NewSlotWidget = Cast<UInventorySlotWidget>(CreateWidget(GetWorld(), InventorySlotBlueprint));
			PlayerInventoryMenuWidgetSlotParent->AddChild(NewSlotWidget);

			UUnrealCraftItem* Item;
			if (PlayerInventory->HasItem(FIntVector2(x, y), Item))
			{
				auto NewItemWidget = Cast<UInventoryItemWidget>(CreateWidget(GetWorld(), InventoryItemBlueprint));
				NewSlotWidget->GetItemParent()->AddChild(NewItemWidget);

				NewItemWidget->SetRepresentedItem(Item);
			}
		}
	}

	this->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryVisualizerWidget::HideSecondaryInventory()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryVisualizerWidget::HidePlayerInventory()
{
	PlayerInventoryMenuWidgetSlotParent->ClearChildren();
	this->SetVisibility(ESlateVisibility::Hidden);
}
