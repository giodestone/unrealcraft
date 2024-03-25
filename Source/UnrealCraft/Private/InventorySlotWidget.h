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
	
	FIntVector2 RepresentedInventoryCoord;
	
	TSharedPtr<IInventoryInterface> AssociatedInventory;
	
public:
	FIntVector2 GetRepresentedInventoryCoord() const;

private:
	UPROPERTY()
	TObjectPtr<UInventoryVisualizerWidget> OwningInventoryVisualizer;

	UPROPERTY()
	TObjectPtr<UInventoryItemWidget> CurrentItemWidget;
	
public:
	void InitializeData(FIntVector2 InRepresentedInventoryCoord, UInventoryVisualizerWidget* InOwningInventoryVisualizer, TSharedPtr<IInventoryInterface> InAssociatedInventory);

	/**
	 * Add a widget to be held by this widget.
	 */
	void AddItemWidget(UInventoryItemWidget* WidgetToAdd);

	/**
	 * Remove the currently held item widget.
	 */
	TObjectPtr<UInventoryItemWidget> RemoveItemWidget();

	/**
	 * Get the currently held widget (if any).
	 */
	UInventoryItemWidget* GetCurrentWidget() const;

	bool IsHoldingWidget() const;

	TSharedPtr<IInventoryInterface> GetAssociatedInventory() const;

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnButtonClicked();
};
