// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

#include "ItemDatabase.h"
#include "UnrealCraftItem.h"
#include "VoxelGameState.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemWidget::SetRepresentedItem(TObjectPtr<UUnrealCraftItem> NewRepresentedItem)
{
	this->RepresentedItem = NewRepresentedItem;

	if (IconImage == nullptr || StackText == nullptr)
		return;
	
	auto ItemInfoDatabase = Cast<AVoxelGameState>(GetWorld()->GetGameState())->GetItemInfoDatabase();
	auto RepresentedItemInfo = ItemInfoDatabase->GetInfo(NewRepresentedItem->GetAssociatedItemID());

	FSlateImageBrush ImageBrush(RepresentedItemInfo.ItemThumbnail->GetFName(),
		FVector2f(RepresentedItemInfo.ItemThumbnail->GetSizeX(), RepresentedItemInfo.ItemThumbnail->GetSizeY()));
	IconImage->SetBrush(ImageBrush);
	
	StackText->SetText(FText::AsNumber(NewRepresentedItem->GetCurrentStack()));
}

void UInventoryItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IconImage = Cast<UImage>(GetWidgetFromName(IconImageWidgetName));
	StackText = Cast<UTextBlock>(GetWidgetFromName(StackTextWidgetName));

	if (IconImage == nullptr || StackText == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryItemWidget::NativeOnInitialized]: Components have not been found from the names configured in the blueprint. This widget will not correctly display the represented item. Check blueprint and try again."));
	}
}
