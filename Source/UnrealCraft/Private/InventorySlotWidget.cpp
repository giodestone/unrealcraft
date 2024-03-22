// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "Components/Button.h"

void UInventorySlotWidget::OnButtonClicked()
{
}

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonWidget != nullptr)
	{
		ButtonWidget->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnButtonClicked);
	}
	else
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventorySlotWidget::NativeOnInitialized]: Button reference not set."));
	}
}
