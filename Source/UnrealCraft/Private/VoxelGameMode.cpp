// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelGameMode.h"

#include "ItemDatabase.h"
#include "PlayerHUD.h"
#include "VoxelGameState.h"
#include "VoxelPawn.h"
#include "VoxelPlayerController.h"
#include "PlayerInventory.h"
#include "UnrealCraftItem.h"

AVoxelGameMode::AVoxelGameMode()
{
	this->PlayerControllerClass = AVoxelPlayerController::StaticClass();
	this->DefaultPawnClass = AVoxelPawn::StaticClass();
	this->GameStateClass = AVoxelGameState::StaticClass();
	this->HUDClass = APlayerHUD::StaticClass();
}

void AVoxelGameMode::StartPlay()
{
	Super::StartPlay();
	
}

void AVoxelGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GetWorld() == nullptr)
		return;

	if (GetWorld()->GetGameState() == nullptr)
		return;
	
	auto VoxelGameState = dynamic_cast<AVoxelGameState*>(GetWorld()->GetGameState());
	
	// TODO: If this game ever goes multiplayer - this needs to be fixed.
	
	if (VoxelGameState == nullptr)
		return;
	
	auto CreatedPlayerInventory = MakeShared<PlayerInventory>();
	VoxelGameState->GetInventoryDatabase().AddEntityInventory("Player", CreatedPlayerInventory);
	VoxelGameState->SetPlayerInventory(CreatedPlayerInventory);

	AddInitialItems(&CreatedPlayerInventory.Get());

	// GLog->Logf(ELogVerbosity::Log, TEXT("[]: New Player Joined, name: %s"), NewPlayer->GetName().GetCharArray());
}

void AVoxelGameMode::AddInitialItems(IInventoryInterface* TargetInventory)
{
	if (GetWorld() == nullptr)
		return;

	if (GetWorld()->GetGameState() == nullptr)
		return;

	auto VoxelGameState = dynamic_cast<AVoxelGameState*>(GetWorld()->GetGameState());

	for (TTuple<FString, FUnrealCraftItemInfo> Item : VoxelGameState->GetItemInfoDatabase()->GetItems())
	{
		auto NewItem = NewObject<UUnrealCraftItem>();
		NewItem->Initialize(Item.Key, Item.Value.MaxStackSize);
		TargetInventory->InsertAnywhereStacked(NewItem);
	}
}
