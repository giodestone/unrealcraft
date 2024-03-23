// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryVisualizerWidget.h"

#include "InventoryInterface.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "VoxelGameState.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"

void UInventoryVisualizerWidget::TogglePlayerInventory(TSharedPtr<IInventoryInterface> PlayerInventory, bool& OutIsMenuDisplayed)
{
	switch (State)
	{
	case Hidden:
		InitPlayerInventoryWidget(PlayerInventory);

		this->CurrentPlayerInventory = PlayerInventory;
		OutIsMenuDisplayed = true;

		State = ShowingPlayer;
		break;
		
	case ShowingBoth: // TODO: Identical to ToggleBothInventories
		HideSecondaryInventory();
		CurrentOtherInventory = nullptr;
	case ShowingPlayer:
		HidePlayerInventory();
		this->CurrentPlayerInventory = nullptr;
		OutIsMenuDisplayed = false;
		State = Hidden;
		break;
		
	}
	// TArray<UWidget*> Children;
	// PlayerInventoryMenuWidget->WidgetTree->GetAllWidgets(Children);
}

void UInventoryVisualizerWidget::ToggleBothInventories(TSharedPtr<IInventoryInterface> PlayerInventory,
                                                     TSharedPtr<IInventoryInterface> OtherInventory,
                                                     bool& OutIsMenuDisplayed)
{
	switch (State)
	{
	case Hidden:
		InitPlayerInventoryWidget(PlayerInventory);
		InitOtherInventoryWidget(OtherInventory);

		this->CurrentPlayerInventory = PlayerInventory;
		this->CurrentOtherInventory = OtherInventory;

		OutIsMenuDisplayed = true;
		State = ShowingBoth;
		break;

	case ShowingBoth: // TODO: Identical to TogglePlayerInventory
		HideSecondaryInventory();
		CurrentOtherInventory = nullptr;
	case ShowingPlayer:
		HidePlayerInventory();
		this->CurrentPlayerInventory = nullptr;
		OutIsMenuDisplayed = false;
		State = Hidden;
		break;
	}
}

void UInventoryVisualizerWidget::Hide()
{
}

void UInventoryVisualizerWidget::OnSlotButtonClicked(UInventorySlotWidget* Widget)
{
	if (CurrentHeldItem == nullptr && Widget->IsHoldingWidget())
	{
		CurrentHeldItem = Widget->RemoveItemWidget();
		HeldItemParentPanel->AddChild(CurrentHeldItem);

		UUnrealCraftItem* RemovedItem; // unused because it is already part of CurrentHeldItem.
		CurrentPlayerInventory->RemoveFrom(Widget->GetRepresentedInventoryCoord(), RemovedItem);
	}
	else if (CurrentHeldItem != nullptr && !Widget->IsHoldingWidget())
	{
		CurrentHeldItem->SetRenderTranslation(FVector2D::Zero());
		CurrentHeldItem->RemoveFromParent();
		Widget->AddItemWidget(CurrentHeldItem);
		CurrentHeldItem = nullptr;
	}
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
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Player inventory widget not set. Check blueprint."));
	}

	HeldItemParentPanel = Cast<UPanelWidget>(GetWidgetFromName(HeldItemParentWidgetName));
	if (HeldItemParentPanel == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Unable to find the parent for held item. Check blueprint."));
	
	Cast<AVoxelGameState>(GetWorld()->GetGameState())->SetInventoryVisualizer(this);
	
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryVisualizerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryVisualizerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TrackCurrentlyHeldItem();
}


void UInventoryVisualizerWidget::InitPlayerInventoryWidget(TSharedPtr<IInventoryInterface> PlayerInventory)
{
	SpawnInventoryGrid(PlayerInventory, PlayerInventoryMenuWidget, PlayerInventoryMenuWidgetSlotParent, InventorySlotBlueprint, InventoryItemBlueprint);

	this->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryVisualizerWidget::InitOtherInventoryWidget(TSharedPtr<IInventoryInterface> OtherInventory)
{
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

void UInventoryVisualizerWidget::TrackCurrentlyHeldItem()
{
	if (CurrentHeldItem != nullptr)
	{
		if (auto PlayerController = GetWorld()->GetFirstPlayerController()) // false if no player controller
		{
			float X, Y;
			if (PlayerController->GetMousePosition(X, Y)) // false if no mouse.
				CurrentHeldItem->SetRenderTranslation(FVector2D(X, Y));
		}
	}
}

void UInventoryVisualizerWidget::SpawnInventoryGrid(TSharedPtr<IInventoryInterface> Inventory, UUserWidget* GridMenuWidget, UPanelWidget* SlotParent, TSubclassOf<UInventorySlotWidget> SlotBlueprint, TSubclassOf<UInventoryItemWidget> ItemBlueprint)
{
	for (int32 x = 0; x < Inventory->GetSize().X; x++)
	{
		for (int32 y = 0; y < Inventory->GetSize().Y; y++)
		{
			auto NewSlotWidget = Cast<UInventorySlotWidget>(CreateWidget(GetWorld(), SlotBlueprint));
			NewSlotWidget->InitializeData(FIntVector2(x,y), this, GridMenuWidget);
			SlotParent->AddChild(NewSlotWidget);

			UUnrealCraftItem* Item;
			if (Inventory->HasItem(FIntVector2(x, y), Item))
			{
				auto NewItemWidget = Cast<UInventoryItemWidget>(CreateWidget(GetWorld(), ItemBlueprint));
				NewSlotWidget->AddItemWidget(NewItemWidget);
				NewItemWidget->SetRepresentedItem(Item);
			}
		}
	}
}