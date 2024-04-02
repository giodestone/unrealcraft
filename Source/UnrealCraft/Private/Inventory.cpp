// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "CoreMinimal.h"
#include "InventoryVisualizerWidget.h"
#include "VoxelGameState.h"
#include "InventoryInterface.h"
#include "ItemDatabase.h"
#include "UnrealCraftItem.h"

Inventory::Inventory()
{
	SetSize(GetDefaultSize());
}

Inventory::~Inventory()
{
}

bool Inventory::InsertAnywhereStacked(UUnrealCraftItem* Item)
{
	// while numToInsert > 0
	//	if found coord of non full stack
	//		insert all possible into that stack
	//		numToInsert = numToInsert - num inserted
	//	else if found empty coord
	//		create new item with numToInsert
	//		insert to empty coord
	//	else
	//		return false as cannot insert anywhere

	
	int8 NumToInsert = Item->GetCurrentStack();

	while (NumToInsert > 0)
	{
		FIntVector2 NonFullStackCoord;
		FIntVector2 FirstEmptyCoord;
		
		if (GetCoordOfNonFullStack(Item, NonFullStackCoord))
		{
			if (GEngine == nullptr)
				return false;
			
			if (GEngine->GetCurrentPlayWorld() == nullptr)
				return false;

			auto ItemDatabase = Cast<AVoxelGameState>(GEngine->GetCurrentPlayWorld()->GetGameState())->GetItemInfoDatabase();
			auto CurrentItemInfo = ItemDatabase->GetInfo(Item->GetAssociatedItemID());

			auto NonFullStackedItem = Items[NonFullStackCoord];

			// 64 items to insert into a 61 stack = 3 items inserted
			// abs(61 - 64) = 3
			// 3 to insert

			// 2 to insert into a 63 stack = 1 item inserted
			// it takes 1 to reach to 64
			// so between 1 and 2, itll take 1 inserted

			// 64 items to insert into a 61 stack means 3 items inserted
			// 3 items to insert into stack before full
			//	max insert = max stack size - current stack
			//	if to be inserted > max insert
			//		to be inserted = max insert
			//	else
			//		to be inserted = stack size

			const int8 MaxToInsert = CurrentItemInfo.MaxStackSize - NonFullStackedItem->GetCurrentStack();
			int8 SubtotalToInsert; 

			if (NumToInsert > MaxToInsert)
				SubtotalToInsert = MaxToInsert;
			else
				SubtotalToInsert = NumToInsert;
			

			if (!NonFullStackedItem->SetCurrentStackUnchecked(SubtotalToInsert + NonFullStackedItem->GetCurrentStack()))
			{
				// something must have went tragically wrong for the code to enter here.
				GLog->Log(ELogVerbosity::Error, TEXT("[Inventory::InsertAnywhere]: Unable to check stack size. Stack size will not be modified."));
				return false;
			}

			NumToInsert -= SubtotalToInsert;
		}
		else if (GetFirstEmptyCoord(FirstEmptyCoord))
		{
			auto NewlyStackedItem = NewObject<UUnrealCraftItem>();
			NewlyStackedItem->Initialize(Item->GetAssociatedItemID(), NumToInsert);
			Items.Add(FirstEmptyCoord, NewlyStackedItem);
			NumToInsert = 0;
		}
		else
		{
			return false;
		}
	}

	return NumToInsert == 0;
	
	// FIntVector2 OutCoord
	// if (GetCoordOfNonFullStack())
	//
	// // find if the inventory contains a specific item.
	// // if so, add to it if stack allows,
	// // otherwise, return the first avaliable item
	// FIntVector2 PreferedSlot(0,0);
	//
	// // while x,y ! greater than size
	// for (int32 x = 0; x < GetSize().X; x++)
	// {
	// 	for (int32 y = 0; y < GetSize().Y; y++)
	// 	{
	// 		PreferedSlot = FIntVector2(x,y);
	//
	// 		if (!Item)
	// 		
	// 		if (Items.Contains(PreferedSlot))
	// 		{
	// 			if (Items[PreferedSlot]->GetAssociatedItemID() == Item->GetAssociatedItemID())
	// 			{
	// 				
	// 			}
	// 		}
	// 		
	// 		if (!Items.Contains(PreferedSlot))
	// 			return InsertInto(PreferedSlot, Item);
	// 	}
	// }
	//
	// return false;
}

bool Inventory::InsertInto(const FIntVector2& Coord, UUnrealCraftItem* Item)
{
	if (!IsCoordInBounds(Coord))
		return false;

	if (Items.Contains(Coord))
		return false;
	
	// TODO: implement stacking
	
	Items.Add(Coord, Item);

	return true;
}

bool Inventory::RemoveFrom(const FIntVector2& Coord, UUnrealCraftItem*& OutItem)
{
	TObjectPtr<UUnrealCraftItem> RemovedItem;
	auto ReturnValue = Items.RemoveAndCopyValue(Coord, RemovedItem);
	OutItem = RemovedItem;
	return ReturnValue;
}

const FIntVector2 Inventory::GetSize() const
{
	return InventorySize;
}

bool Inventory::HasItem(const FIntVector2& Coord, UUnrealCraftItem*& OutItem) const
{
	if (!IsCoordInBounds(Coord))
		return false;

	if (!Items.Contains(Coord))
		return false;

	OutItem = Items[Coord];
	return true;
}

bool Inventory::RemoveNumberFrom(const FIntVector2& Coord, const int8 NumToRemove, UUnrealCraftItem*& OutItem)
{
	UUnrealCraftItem* HasItemOut;
	if (!HasItem(Coord, HasItemOut))
		return false;

	if (HasItemOut->GetCurrentStack() < NumToRemove)
		return false;

	if (HasItemOut->GetCurrentStack() == NumToRemove)
		return RemoveFrom(Coord, OutItem);

	// update current stack size
	if (!HasItemOut->SetCurrentStackUnchecked(HasItemOut->GetCurrentStack() - NumToRemove))
		return false;

	// create new item with the remaining stack.
	OutItem = NewObject<UUnrealCraftItem>();
	OutItem->Initialize(HasItemOut->GetAssociatedItemID(), NumToRemove);
	
	return true;
}

void Inventory::SetSize(FIntVector2 Size)
{
	for (auto ItemTuple : Items)
	{
		if (ItemTuple.Key.X > Size.X || ItemTuple.Key.Y > Size.Y)
		{
			// TODO: Support inventory size decrease.
			GLog->Log(ELogVerbosity::Error, TEXT("[Inventory::SetSize]: Decrease of inventory size is currently unsupported."));
			break;
		}
	}
	
	this->InventorySize = Size;
}

FIntVector2 Inventory::GetDefaultSize() const
{
	return FIntVector2(9, 4);
}

bool Inventory::IsCoordInBounds(const FIntVector2& Coord) const
{
	return Coord.X < InventorySize.X && Coord.Y < InventorySize.Y && Coord.X >= 0 && Coord.Y >= 0;
}

bool Inventory::GetCoordOfNonFullStack(const UUnrealCraftItem* Item, FIntVector2& OutCoord) const
{
	for (int32 x = 0; x < GetSize().X; x++)
	{
		for (int32 y = 0; y < GetSize().Y; y++)
		{
			FIntVector2 CurrentCoord(x,y);

			if (!Items.Contains(CurrentCoord))
				continue;

			if (Items[CurrentCoord]->GetAssociatedItemID() != Item->GetAssociatedItemID())
				continue;

			if (GEngine == nullptr)
				return false;
			
			if (GEngine->GetCurrentPlayWorld() == nullptr)
				return false;
			
			auto CurrentItemInfo = Cast<AVoxelGameState>(GEngine->GetCurrentPlayWorld()->GetGameState())->GetItemInfoDatabase()->GetInfo(Item->GetAssociatedItemID());

			if (Items[CurrentCoord]->GetCurrentStack() >= CurrentItemInfo.MaxStackSize)
				continue;

			OutCoord = CurrentCoord;
			return true;
		}
	}

	return false;
}

bool Inventory::GetFirstEmptyCoord(FIntVector2& OutCoord) const
{
	// TODO: Consider optimising by adding start x, start y; end x, end y.
	
	for (int32 x = 0; x < GetSize().X; x++)
	{
		for (int32 y = 0; y < GetSize().Y; y++)
		{
			FIntVector2 CurrentCoord(x,y);

			if (!Items.Contains(CurrentCoord))
			{
				OutCoord = CurrentCoord;
				return true;
			}
		}
	}

	return false;
}
