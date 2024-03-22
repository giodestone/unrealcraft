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

	EInventoryVisualiserState State = Hidden;

	UPROPERTY(EditAnywhere, Category="Blueprints")
	TSubclassOf<UInventorySlotWidget> InventorySlotBlueprint;

	UPROPERTY(EditAnywhere, Category="Blueprints")
	TSubclassOf<UInventoryItemWidget> InventoryItemBlueprint;

	UPROPERTY(EditAnywhere, Category="References")
	FName PlayerInventoryMenuWidgetName = "PlayerInventoryGrid";

	UPROPERTY(EditAnywhere, Category="References")
	TObjectPtr<UUserWidget> OtherInventorySlotWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerInventoryMenuWidget;
	UPROPERTY()
	TObjectPtr<UPanelWidget> PlayerInventoryMenuWidgetSlotParent;

public:
	void TogglePlayerInventory(TSharedPtr<IInventoryInterface> PlayerInventory, bool& OutIsMenuDisplayed);
	void ShowBothInventories(IInventoryInterface* PlayerInventory, IInventoryInterface* OtherInventory);
	void Hide();

protected:
	/**
	 * Callled once when the game is started.
	 */
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	void InitPlayerInventoryWidget(TSharedPtr<IInventoryInterface> PlayerInventory);
	void HideSecondaryInventory();
	void HidePlayerInventory();
};
