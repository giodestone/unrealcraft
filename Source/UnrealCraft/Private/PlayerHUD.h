// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryVisualizerWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerHotbarWidget;
class UInventoryVisualizerWidget;

/**
 * Manages and represents the players' HUD.
 */
UCLASS()
class APlayerHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Blueprints")
	TSubclassOf<UUserWidget> CrosshairWidget = UUserWidget::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category="Blueprints")
	TSubclassOf<UInventoryVisualizerWidget> InventoryScreenWidgetBlueprint;
	
	UPROPERTY(EditDefaultsOnly, Category="Blueprints")
	TSubclassOf<UPlayerHotbarWidget> HotbarWidgetBlueprint;

	
	UPROPERTY()
	TObjectPtr<UPlayerHotbarWidget> HotbarWidget;

private:
	UPROPERTY()
	TObjectPtr<UInventoryVisualizerWidget> InventoryScreenWidget;

public:
	/**
	 * Get the reference to the inventory screen.
	 */
	TObjectPtr<UInventoryVisualizerWidget>& GetInventoryScreenWidget() { return InventoryScreenWidget; }

	/**
	 * Get the hotbar widget.
	 */
	TObjectPtr<UPlayerHotbarWidget> GetHotbarWidget() const;

	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;
};
