// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class IInventoryInterface;
class UInventoryItemWidget;
class UInventoryVisualizerWidget;
class UButton;
/**
 * Represents an inventory slot, which {@link UInventoryItemWidget} can dock into.
 */
UCLASS()
class UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	FName ButtonWidgetName = "SlotButton";
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPanelWidget> ItemParent;

	UPROPERTY()
	TObjectPtr<UButton> ButtonWidget;

	UPROPERTY()
	TObjectPtr<UInventoryVisualizerWidget> OwningInventoryVisualizer;

	UPROPERTY()
	TObjectPtr<UInventoryItemWidget> CurrentItemWidget;
	
	FIntVector2 RepresentedInventoryCoord;
	
	TSharedPtr<IInventoryInterface> AssociatedInventory;
	
public:
	/**
	 * Get the coordinate this slot represents inside the associated inventory.
	 */
	FIntVector2 GetRepresentedInventoryCoord() const;

	/**
	 * Get the currently held widget (if any).
	 */
	UInventoryItemWidget* GetCurrentWidget() const;

	/**
	 * Get the inventory this widget represents.
	 */
	TSharedPtr<IInventoryInterface> GetAssociatedInventory() const;
	
	/**
	 * Whether this slot currently contains an item widget.
	 */
	bool IsHoldingWidget() const;

	/**
	 * Initialize this component for the first time. This must be called.
	 * @param InRepresentedInventoryCoord The coordinate that this inventory represents in the associated inventory.
	 * @param InOwningInventoryVisualizer Which inventory visualizer this slot belongs to.
	 * @param InAssociatedInventory The associated inventory this slot represents.
	 */
	void InitializeData(FIntVector2 InRepresentedInventoryCoord, UInventoryVisualizerWidget* InOwningInventoryVisualizer, TSharedPtr<IInventoryInterface> InAssociatedInventory);

	/**
	 * Add a widget to be held by this widget.
	 */
	void AddItemWidget(UInventoryItemWidget* WidgetToAdd);

	/**
	 * Remove the currently held item widget.
	 */
	TObjectPtr<UInventoryItemWidget> RemoveItemWidget();

protected:
	virtual void NativeOnInitialized() override;

private:
	/**
	 * Callback for when the button is clicked.
	 */
	UFUNCTION()
	void OnButtonClicked();
};
