// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryVisualizerWidget.generated.h"

class UInventoryItemWidget;
class UInventorySlotWidget;
class IInventoryInterface;

UENUM()
enum EInventoryVisualiserState
{
	Hidden, ShowingPlayer, ShowingBoth
};

/**
 * For visualising the players or world inventory on the screen.
 */
UCLASS()
class UInventoryVisualizerWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	EInventoryVisualiserState State = Hidden;

	UPROPERTY(EditAnywhere, Category="Blueprints")
	TSubclassOf<UInventorySlotWidget> InventorySlotBlueprint;

	UPROPERTY(EditAnywhere, Category="Blueprints")
	TSubclassOf<UInventoryItemWidget> InventoryItemBlueprint;

	UPROPERTY(EditAnywhere, Category="References")
	FName PlayerInventoryMenuWidgetName = "PlayerInventoryGrid";

	UPROPERTY(EditDefaultsOnly, Category="References")
	FName HeldItemParentWidgetName = "HeldItemParent";
	
	UPROPERTY(EditAnywhere, Category="References")
	TObjectPtr<UUserWidget> OtherInventorySlotWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerInventoryMenuWidget;

	UPROPERTY()
	TObjectPtr<UPanelWidget> PlayerInventoryMenuWidgetSlotParent;

	UPROPERTY()
	TObjectPtr<UPanelWidget> HeldItemParentPanel;

	UPROPERTY()
	TObjectPtr<UInventoryItemWidget> CurrentHeldItem;

	TSharedPtr<IInventoryInterface> CurrentPlayerInventory;
	TSharedPtr<IInventoryInterface> CurrentOtherInventory;

	// if slot clicked and item is in it
	// give the item to the mouse
	// if another click on a slot is done, and we have an item.
	// place it into the slot.

public:
	void TogglePlayerInventory(TSharedPtr<IInventoryInterface> PlayerInventory, bool& OutIsMenuDisplayed);
	void ToggleBothInventories(TSharedPtr<IInventoryInterface> PlayerInventory, TSharedPtr<IInventoryInterface> OtherInventory, bool& OutIsMenuDisplayed);
	void Hide();

	UFUNCTION()
	void OnSlotButtonClicked(UInventorySlotWidget* Widget);
	
protected:
	/**
	 * Callled once when the game is started.
	 */
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void InitPlayerInventoryWidget(TSharedPtr<IInventoryInterface> PlayerInventory);
	void InitOtherInventoryWidget(TSharedPtr<IInventoryInterface> OtherInventory);
	void HideSecondaryInventory();
	void HidePlayerInventory();

	/**
	 * Update the currently held items' position to track the mouse.
	 * @remarks Should bec called in {@link NativeTick}.
	 */
	void TrackCurrentlyHeldItem();

	/**
	 * Spawn an inventory grid by creating slots and populating them with items from the inventory.
	 * @param Inventory The inventory with the items to spawn.
	 * @param GridMenuWidget The 'owning' widget that the grid represents.
	 * @param SlotParent Where the slots should be added to.
	 * @param SlotBlueprint The slots that will be spawned.
	 * @param ItemBlueprint The items that would be put into the slots, if they are in the inventory.
	 */
	void SpawnInventoryGrid(TSharedPtr<IInventoryInterface> Inventory, UUserWidget* GridMenuWidget, UPanelWidget* SlotParent, TSubclassOf<UInventorySlotWidget> SlotBlueprint, TSubclassOf<UInventoryItemWidget> ItemBlueprint);

};
