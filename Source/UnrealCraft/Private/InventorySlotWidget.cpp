// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "InventoryItemWidget.h"
#include "InventoryVisualizerWidget.h"
#include "Components/Button.h"

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (SlotButton == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventorySlotWidget::NativeOnInitialized]: Button reference not set. Check the blueprint."));
	else
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnButtonClicked);
}

void UInventorySlotWidget::OnButtonClicked()
{
	OwningInventoryVisualizer->OnSlotButtonClicked(this);
}

TObjectPtr<UPanelWidget> UInventorySlotWidget::GetHotbarCursorParent() const
{
	return HotbarCursorParent;
}

FIntVector2 UInventorySlotWidget::GetRepresentedInventoryCoord() const
{
	return RepresentedInventoryCoord;
}

void UInventorySlotWidget::InitializeData(FIntVector2 InRepresentedInventoryCoord, UInventoryVisualizerWidget* InOwningInventoryVisualizer, TSharedPtr<IInventoryInterface> InAssociatedInventory)
{
	this->RepresentedInventoryCoord = InRepresentedInventoryCoord;
	OwningInventoryVisualizer = InOwningInventoryVisualizer;
	AssociatedInventory = InAssociatedInventory;
}

void UInventorySlotWidget::AddItemWidget(UInventoryItemWidget* WidgetToAdd)
{
	ItemParent->AddChild(WidgetToAdd); // this order is really important!
	this->CurrentItemWidget = WidgetToAdd;
}

TObjectPtr<UInventoryItemWidget> UInventorySlotWidget::RemoveItemWidget()
{
	CurrentItemWidget->RemoveFromParent();
	TObjectPtr<UInventoryItemWidget> RemovedWidget = CurrentItemWidget; // so we can keep a reference to the memory address without breaking anything.
	CurrentItemWidget = nullptr;
	return RemovedWidget;
}

UInventoryItemWidget* UInventorySlotWidget::GetCurrentWidget() const
{
	return CurrentItemWidget;
}

bool UInventorySlotWidget::IsHoldingWidget() const
{
	return GetCurrentWidget() != nullptr;
}

TSharedPtr<IInventoryInterface> UInventorySlotWidget::GetAssociatedInventory() const
{
	return AssociatedInventory;
}
