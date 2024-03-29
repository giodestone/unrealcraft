// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHotbarWidget.generated.h"

class UHotbarCursorWidget;
class UInventoryVisualizerWidget;
class UInventoryItemWidget;
class UInventorySlotWidget;
class PlayerInventory;
/**
 * Represents the contents of the player's hotbar on screen.
 */
UCLASS()
class UPlayerHotbarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="References")
	TSubclassOf<UInventorySlotWidget> SlotBlueprint;

	UPROPERTY(EditDefaultsOnly, Category="References")
	TSubclassOf<UInventoryItemWidget> ItemBlueprint;

	UPROPERTY(Transient, VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<UHotbarCursorWidget> HotbarCursor;
	
	
	UPROPERTY(Transient, VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<UPanelWidget> SlotParent;

	UPROPERTY()
	TObjectPtr<UInventoryVisualizerWidget> InventoryVisualizerWidget;
	
	TSharedPtr<PlayerInventory> RepresentedPlayerInventory;

	TArray<TObjectPtr<UInventorySlotWidget>> CreatedSlots;
public:
	/**
	 * Update the hotbar cursor to reflect a newly selected slot.
	 */
	void UpdateCursorPosition(int32 TargetSelectedSlot);
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/**
	 * Spawn one row of slots representing the hotbar portion of the inventory.
	 */
	void SpawnSlotsAndItems();
};
