// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "PlayerHotbarWidget.h"
#include "Blueprint/UserWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	// Z matters! This ensures things are above/below where they need to be.
	// * Crosshair must be below any top level UI
	// * Hotbar widget must be below inventory UI
	// * Inventory UI must be on top of any game UI
	
	auto InstancedCrosshairWidget = CreateWidget(GetWorld(), CrosshairWidget);

	if (InstancedCrosshairWidget != nullptr)
		InstancedCrosshairWidget->AddToViewport(-1);

	if (InventoryScreenWidgetBlueprint == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[APlayerHUD::BeginPlay]: InventoryScreenWidgetBlueprint not set. Will not instanciate inventory widget."));

	
	InventoryScreenWidget = CreateWidget<UInventoryVisualizerWidget>(GetWorld(), InventoryScreenWidgetBlueprint);

	if (InventoryScreenWidget == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[APlayerHUD::BeginPlay]: Unable to instance inventory screen."));
	else
	{
		InventoryScreenWidget->AddToViewport(1);
	}
	
	HotbarWidget = CreateWidget<UPlayerHotbarWidget>(GetWorld(), HotbarWidgetBlueprint);

	if (HotbarWidget == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[APlayerHUD::BeginPlay]: Unable to instance inventory screen."));
	else
	{
		HotbarWidget->AddToViewport(0);
	}
}

TObjectPtr<UPlayerHotbarWidget> APlayerHUD::GetHotbarWidget() const
{
	return HotbarWidget;
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
}
