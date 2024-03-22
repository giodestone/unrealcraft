// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

#include "ItemDatabase.h"
#include "VoxelGameState.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemWidget::SetRepresentedItem(TObjectPtr<UUnrealCraftItem> NewRepresentedItem)
{
	this->RepresentedItem = NewRepresentedItem;

	auto ItemInfoDatabase = Cast<AVoxelGameState>(GetWorld()->GetGameState())->GetItemInfoDatabase();
	auto RepresentedItemInfo = ItemInfoDatabase->GetInfo(NewRepresentedItem->GetAssociatedItemID());

	FSlateImageBrush ImageBrush(RepresentedItemInfo.ItemThumbnail->GetFName(),
		FVector2f(RepresentedItemInfo.ItemThumbnail->GetSizeX(), RepresentedItemInfo.ItemThumbnail->GetSizeY()));
	IconImage->SetBrush(ImageBrush);
	
	StackText->SetText(FText::AsNumber(NewRepresentedItem->GetCurrentStack()));
}
