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
		State = ShowingPlayer;
		OutIsMenuDisplayed = true;
		this->CurrentPlayerInventory = PlayerInventory;
		break;
		
	case ShowingBoth:
		HideSecondaryInventory();
	case ShowingPlayer:
		HidePlayerInventory();
		State = Hidden;
		CurrentPlayerInventory = nullptr;
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

void UInventoryVisualizerWidget::InitPlayerInventoryWidget(TSharedPtr<IInventoryInterface> PlayerInventory)
{
	for (int32 x = 0; x < PlayerInventory->GetSize().X; x++)
	{
		for (int32 y = 0; y < PlayerInventory->GetSize().Y; y++)
		{
			auto NewSlotWidget = Cast<UInventorySlotWidget>(CreateWidget(GetWorld(), InventorySlotBlueprint));
			NewSlotWidget->InitializeData(FIntVector2(x,y), this);
			PlayerInventoryMenuWidgetSlotParent->AddChild(NewSlotWidget);

			UUnrealCraftItem* Item;
			if (PlayerInventory->HasItem(FIntVector2(x, y), Item))
			{
				auto NewItemWidget = Cast<UInventoryItemWidget>(CreateWidget(GetWorld(), InventoryItemBlueprint));
				NewSlotWidget->AddItemWidget(NewItemWidget);
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

void UInventoryVisualizerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TrackCurrentlyHeldItem();
}
