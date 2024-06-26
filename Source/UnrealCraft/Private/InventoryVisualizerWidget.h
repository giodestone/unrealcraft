// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryVisualizerWidget.generated.h"

class UButton;
class UInventoryItemWidget;
class UInventorySlotWidget;
class IInventoryInterface;
class UPlayerInventory;

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

	UPROPERTY(Transient, Transient, VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<UPanelWidget> OtherInventoryMenuAbsoluteParentWidget; // The one with the text etc.

	UPROPERTY(Transient, Transient, VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> CloseButton; // for closing the GUI.
	
	// TODO: Change this to use BindWidget
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

	UPROPERTY()
	TObjectPtr<UPlayerInventory> CurrentPlayerInventory;

	UPROPERTY()
	TScriptInterface<IInventoryInterface> CurrentOtherInventory;
	
	EInventoryVisualiserState State = Hidden;

	const ESlateVisibility DefaultSelfInvisibility = ESlateVisibility::Hidden;
	const ESlateVisibility DefaultSelfVisibility = ESlateVisibility::SelfHitTestInvisible;

public:
	/**
	 * Toggle display of the player inventory or everything.
	 * @param InPlayerInventory The player inventory's contents to display.
	 * @param OutIsMenuDisplayed Whether something is displayed on screen.
	 */
	void TogglePlayerInventory(TObjectPtr<UPlayerInventory> InPlayerInventory, bool& OutIsMenuDisplayed);

	/**
	 * Toggle the display of both player and a world/entity inventory.
	 * @param InPlayerInventory The player inventory's contents to display. 
	 * @param OtherInventory The other inventory's contents to display.
	 * @param OutIsMenuDisplayed Whether something is displayed on screen.
	 */
	void ToggleBothInventories(TObjectPtr<UPlayerInventory> InPlayerInventory, TScriptInterface<IInventoryInterface> OtherInventory, bool& OutIsMenuDisplayed);

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
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

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
	void InitPlayerInventoryWidget(TObjectPtr<UPlayerInventory> InPlayerInventory);

	/**
	 * Set up a grid of slots and fill with items from the other inventory.
	 */
	void InitOtherInventoryWidget(TScriptInterface<IInventoryInterface> OtherInventory);

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
	void SpawnInventoryGrid(TScriptInterface<IInventoryInterface> Inventory, UUserWidget* GridMenuWidget, UPanelWidget* SlotParent, TSubclassOf<UInventorySlotWidget> SlotBlueprint, TSubclassOf<UInventoryItemWidget> ItemBlueprint, FIntVector2
	                        SizeOffset = FIntVector2(0, 0));

	/**
	 * Function to be called when the close button is clicked.
	 */
	UFUNCTION()
	void OnCloseButtonClicked();
	
	/**
	 * Override, which handles the E or I buttons to hide the screen.
	 */
	FReply KeyboardShortcutHideInventoryScreen(const FKeyEvent& InKeyEvent);
	
};
