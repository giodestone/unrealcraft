// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "PlayerHotbarWidget.h"
#include "Blueprint/UserWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	auto InstancedCrosshairWidget = CreateWidget(GetWorld(), CrosshairWidget);

	if (InstancedCrosshairWidget != nullptr)
		InstancedCrosshairWidget->AddToViewport();

	if (InventoryScreenWidgetBlueprint == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[APlayerHUD::BeginPlay]: InventoryScreenWidgetBlueprint not set. Will not instanciate inventory widget."));

	
	InventoryScreenWidget = CreateWidget<UInventoryVisualizerWidget>(GetWorld(), InventoryScreenWidgetBlueprint);

	if (InventoryScreenWidget == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[APlayerHUD::BeginPlay]: Unable to instance inventory screen."));
	else
	{
		InventoryScreenWidget->AddToViewport();
	}

	
	HotbarWidget = CreateWidget<UPlayerHotbarWidget>(GetWorld(), HotbarWidgetBlueprint);

	if (HotbarWidget == nullptr)
		GLog->Log(ELogVerbosity::Error, TEXT("[APlayerHUD::BeginPlay]: Unable to instance inventory screen."));
	else
	{
		HotbarWidget->AddToViewport();
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
