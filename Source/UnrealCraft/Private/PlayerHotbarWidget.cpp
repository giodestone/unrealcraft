// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHotbarWidget.h"

#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "PlayerInventory.h"
#include "VoxelGameState.h"
#include "Components/PanelWidget.h"

void UPlayerHotbarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotParent = dynamic_cast<UPanelWidget*>(GetWidgetFromName(SlotParentWidgetName));

	if (SlotParent == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[UPlayerHotbarWidget::NativeOnInitialized]: Unable to find slot parent widget. Check blueprint."));

	if (GetWorld() != nullptr)
	{
		if (GetWorld()->GetGameState() != nullptr)
		{
			auto GameState = dynamic_cast<AVoxelGameState*>(GetWorld()->GetGameState());
			RepresentedPlayerInventory = GameState->GetPlayerInventory();
			InventoryVisualizerWidget = GameState->GetInventoryVisualizer();
		}
	}

	SpawnSlotsAndItems();
}

void UPlayerHotbarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerHotbarWidget::SpawnSlotsAndItems()
{
	if (RepresentedPlayerInventory == nullptr || InventoryVisualizerWidget == nullptr)
		return;
	
	int32 y = RepresentedPlayerInventory->GetHotBarRowStartCoords().Y;
	for (int32 x = RepresentedPlayerInventory->GetHotBarRowStartCoords().X; x < RepresentedPlayerInventory->GetSize().X; x++)
	{
		auto NewSlotWidget = Cast<UInventorySlotWidget>(CreateWidget(GetWorld(), SlotBlueprint));
		NewSlotWidget->InitializeData(FIntVector2(x,y), InventoryVisualizerWidget, RepresentedPlayerInventory);
		SlotParent->AddChild(NewSlotWidget);

		UUnrealCraftItem* Item;
		if (RepresentedPlayerInventory->HasItem(FIntVector2(x, y), Item))
		{
			auto NewItemWidget = Cast<UInventoryItemWidget>(CreateWidget(GetWorld(), ItemBlueprint));
			NewSlotWidget->AddItemWidget(NewItemWidget);
			NewItemWidget->SetRepresentedItem(Item);
		}
	}
}
