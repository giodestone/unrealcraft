// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryVisualizerWidget.generated.h"

class UInventoryItemWidget;
class UInventorySlotWidget;
class IInventoryInterface;

UENUM(BlueprintType)
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

	
	TSharedPtr<IInventoryInterface> CurrentPlayerInventory;
	TSharedPtr<IInventoryInterface> CurrentOtherInventory;
	
	EInventoryVisualiserState State = Hidden;

public:
	void TogglePlayerInventory(TSharedPtr<IInventoryInterface> PlayerInventory, bool& OutIsMenuDisplayed);
	void ToggleBothInventories(TSharedPtr<IInventoryInterface> PlayerInventory, TSharedPtr<IInventoryInterface> OtherInventory, bool& OutIsMenuDisplayed);

	/**
	 * Hides all visible UI.
	 */
	UFUNCTION(BlueprintCallable)
	void Hide();

	UFUNCTION()
	void OnSlotButtonClicked(UInventorySlotWidget* Widget);

	UFUNCTION(BlueprintCallable)
	EInventoryVisualiserState GetState() const;

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
