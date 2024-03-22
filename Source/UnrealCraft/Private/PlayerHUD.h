// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryVisualizerWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UInventoryVisualizerWidget;
/**
 * Manages and represents the players' HUD.
 */
UCLASS()
class APlayerHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CrosshairWidget = UUserWidget::StaticClass();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryVisualizerWidget> InventoryScreenWidgetBlueprint;
	//
	// UPROPERTY(EditDefaultsOnly)
	// TSubclassOf<UUserWidget> HotbarWidget;

	UPROPERTY()
	TObjectPtr<UInventoryVisualizerWidget> InventoryScreenWidget;

public:
	TObjectPtr<UInventoryVisualizerWidget>& GetInventoryScreenWidget() { return InventoryScreenWidget; }

	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

private:
	
	
};