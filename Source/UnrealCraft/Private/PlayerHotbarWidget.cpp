// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHotbarWidget.h"

#include "HotbarCursorWidget.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "PlayerInventory.h"
#include "VoxelGameState.h"
#include "Components/PanelWidget.h"

void UPlayerHotbarWidget::UpdateCursorPosition()
{
	int32 TargetSelectedSlot = RepresentedPlayerInventory->GetCurrentlySelectedHotbarSlot();
	
	if (TargetSelectedSlot < 0 || TargetSelectedSlot > CreatedSlots.Num() - 1)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UPlayerHotbarWidget::UpdateCursorPosition]: Arg error, TargetSelectedSlot is out of bounds. Cursor position will not be updated."));
		return;
	}

	if (CreatedSlots[TargetSelectedSlot]->GetHotbarCursorParent() == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UPlayerHotbarWidget::UpdateCursorPosition]: The selected slot does not have a HotbarCursorParent set. Cursor position will not be updated."));
		return;
	}
	
	HotbarCursor->AnimateToNewParent(CreatedSlots[TargetSelectedSlot]->GetHotbarCursorParent());
}

void UPlayerHotbarWidget::UpdateItems()
{
	if (CreatedSlots.Num() == 0)
		return;
	
	int32 y = RepresentedPlayerInventory->GetHotBarRowStartCoords().Y;
	for (int32 x = RepresentedPlayerInventory->GetHotBarRowStartCoords().X; x < RepresentedPlayerInventory->GetHotbarSize().X; x++)
	{
		UInventorySlotWidget* CurrentSlot = CreatedSlots[x];

		if (CurrentSlot->GetCurrentWidget() != nullptr)
			CurrentSlot->RemoveItemWidget(); // dispose of current item.
		
		UUnrealCraftItem* Item;
		if (RepresentedPlayerInventory->HasItem(FIntVector2(x, y), Item))
		{
			auto NewItemWidget = Cast<UInventoryItemWidget>(CreateWidget(GetWorld(), ItemBlueprint));
			CurrentSlot->AddItemWidget(NewItemWidget);
			NewItemWidget->SetRepresentedItem(Item);
		}
	}
}

void UPlayerHotbarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (GetWorld() != nullptr)
	{
		if (GetWorld()->GetGameState() != nullptr)
		{
			auto GameState = dynamic_cast<AVoxelGameState*>(GetWorld()->GetGameState());
			RepresentedPlayerInventory = GameState->GetPlayerInventory();
			InventoryVisualizerWidget = GameState->GetInventoryVisualizer();
		}
	}

	
	SpawnSlots();

	UpdateItems();

	HotbarCursor->AnimateToNewParent(CreatedSlots[0]->GetHotbarCursorParent());
}

void UPlayerHotbarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerHotbarWidget::SpawnSlots()
{
	if (RepresentedPlayerInventory == nullptr || InventoryVisualizerWidget == nullptr)
		return;

	if (CreatedSlots.Num() > 0)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UPlayerHotbarWidget::SpawnSlotsAndItems]: Slots have already been created. This means this function was called twice, and it should not be! No new slots will be created.")); // Though if this becomes an error, add a cleanup function to make it possible to call this multiple times.
		return;
	}

	CreatedSlots.Reserve(RepresentedPlayerInventory->GetHotbarSize().X);
	
	int32 y = RepresentedPlayerInventory->GetHotBarRowStartCoords().Y;
	for (int32 x = RepresentedPlayerInventory->GetHotBarRowStartCoords().X; x < RepresentedPlayerInventory->GetHotbarSize().X; x++)
	{
		auto NewSlotWidget = Cast<UInventorySlotWidget>(CreateWidget(GetWorld(), SlotBlueprint));
		NewSlotWidget->InitializeData(FIntVector2(x,y), InventoryVisualizerWidget, RepresentedPlayerInventory);
		SlotParent->AddChild(NewSlotWidget);
		CreatedSlots.Add(NewSlotWidget);
	}
}
