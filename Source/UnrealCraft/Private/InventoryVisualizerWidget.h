// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryVisualizerWidget.generated.h"

class UInventoryItemWidget;
class UInventorySlotWidget;
class IInventoryInterface;
class PlayerInventory;

/**
 * For representing the visual state of @class UInventoryVisualizerWidget.
 */
UENUM(BlueprintType)
enum EInventoryVisualiserState
{
	Hidden, ShowingPlayer, ShowingBoth
};

/**
 * For visualising the player and/or a world inventory.
 */
UCLASS()
class UInventoryVisualizerWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category="Blueprints")
	TSubclassOf<UInventorySlotWidget> InventorySlotBlueprint;

	UPROPERTY(EditDefaultsOnly, Category="Blueprints")
	TSubclassOf<UInventoryItemWidget> InventoryItemBlueprint;

	// Yes getting them by fname seems counterinutive, but the references keep resetting for some reason.
	UPROPERTY(EditDefaultsOnly, Category="References")
	FName PlayerInventoryMenuWidgetName = "PlayerInventoryGrid";

	UPROPERTY(EditDefaultsOnly, Category="References")
	FName OtherInventoryMenuWidgetName = "OtherInventoryGrid";

	UPROPERTY(EditDefaultsOnly, Category="References")
	FName HeldItemParentWidgetName = "HeldItemParent";

	UPROPERTY(EditDefaultsOnly, Category="References")
	FName InventoryMenuSlotParentName = "InventorySlotParent";
	

	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerInventoryMenuWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> OtherInventoryMenuWidget;

	// TODO: Consider moving this logic to its own InventoryGridWidget class.
	UPROPERTY()
	TObjectPtr<UPanelWidget> PlayerInventoryMenuWidgetSlotParent;
	
	UPROPERTY()
	TObjectPtr<UPanelWidget> OtherInventoryMenuWidgetSlotParent;
	

	UPROPERTY()
	TObjectPtr<UPanelWidget> HeldItemParentPanel;

	UPROPERTY()
	TObjectPtr<UInventoryItemWidget> CurrentHeldItem;

	
	TSharedPtr<PlayerInventory> CurrentPlayerInventory;
	TSharedPtr<IInventoryInterface> CurrentOtherInventory;
	
	EInventoryVisualiserState State = Hidden;

public:
	/**
	 * Toggle display of the player inventory or everything.
	 * @param InPlayerInventory The player inventory's contents to display.
	 * @param OutIsMenuDisplayed Whether something is displayed on screen.
	 */
	void TogglePlayerInventory(TSharedPtr<PlayerInventory> InPlayerInventory, bool& OutIsMenuDisplayed);

	/**
	 * Toggle the display of both player and a world/entity inventory.
	 * @param InPlayerInventory The player inventory's contents to display. 
	 * @param OtherInventory The other inventory's contents to display.
	 * @param OutIsMenuDisplayed Whether something is displayed on screen.
	 */
	void ToggleBothInventories(TSharedPtr<PlayerInventory> InPlayerInventory, TSharedPtr<IInventoryInterface> OtherInventory, bool& OutIsMenuDisplayed);

	/**
	 * Hides all visible UI.
	 */
	UFUNCTION(BlueprintCallable)
	void Hide();

	/**
	 * For {@link UInventorySlotWidget} to call when it's clicked on.
	 */
	UFUNCTION()
	void OnSlotButtonClicked(UInventorySlotWidget* Widget);

	/**
	 * Get the current state of the widget.
	 */
	UFUNCTION(BlueprintCallable)
	EInventoryVisualiserState GetState() const;

	/**
	 * Redirect player's input to the world.
	 * @remark Does not call @link Hide@endlink
	 */
	UFUNCTION(BlueprintCallable)
	void ChangePlayerInputsToWorld();

protected:
	/**
	 * Callled once when the game is started.
	 */
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/**
	 * Get the components in this object from name.
	 */
	void SetupComponentsFromName();

	/**
	 * Get a panel sub widget pointer from the ParentWidget by name.
	 * @return nullptr if not found. Logs an error too.
	 */
	UPanelWidget* GetPanelSubWidget(UUserWidget* ParentWidget, const FName& PanelWidgetName);

	/**
	 * Set up a grid of slots and fill with items from the player. 
	 */
	void InitPlayerInventoryWidget(TSharedPtr<PlayerInventory> InPlayerInventory);

	/**
	 * Set up a grid of slots and fill with items from the other inventory.
	 */
	void InitOtherInventoryWidget(TSharedPtr<IInventoryInterface> OtherInventory);

	/**
	 * Hide the secondary inventory and destory the slots.
	 */
	void HideSecondaryInventory();

	/**
	 * Hide the player inventory and destory the slots.
	 */
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
	 * @param SizeOffset This value is added to the {@link Inventory.GetSize()}. Can be used to increase/decrease the values.
	 */
	void SpawnInventoryGrid(TSharedPtr<IInventoryInterface> Inventory, UUserWidget* GridMenuWidget, UPanelWidget* SlotParent, TSubclassOf<UInventorySlotWidget> SlotBlueprint, TSubclassOf<UInventoryItemWidget> ItemBlueprint, FIntVector2
	                        SizeOffset = FIntVector2(0, 0));

protected:
	/**
	 * Override, which handles the E or I buttons to hide the screen.
	 */
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
