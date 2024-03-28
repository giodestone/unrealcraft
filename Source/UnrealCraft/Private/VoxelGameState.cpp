// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelGameState.h"

#include "GameFramework/PlayerState.h"
#include "PlayerInventory.h"
#include "InventoryVisualizerWidget.h"

InventoryDatabase& AVoxelGameState::GetInventoryDatabase()
{
	return InventoryDatabase;
}

TObjectPtr<UInventoryVisualizerWidget> AVoxelGameState::GetInventoryVisualizer() const
{ return InventoryVisualizer; }

TSharedPtr<PlayerInventory> AVoxelGameState::GetPlayerInventory() const
{
	return MainPlayerInventory;
}

void AVoxelGameState::SetInventoryVisualizer(TObjectPtr<UInventoryVisualizerWidget> NewInventoryVisualizer)
{
	InventoryVisualizer = NewInventoryVisualizer;
}

void AVoxelGameState::SetPlayerInventory(TSharedPtr<PlayerInventory> NewPlayerInventory)
{
	MainPlayerInventory = NewPlayerInventory;
}

TObjectPtr<UItemInfoDatabase> AVoxelGameState::GetItemInfoDatabase() const
{
	return ItemInfoDatabase;
}

void AVoxelGameState::BeginPlay()
{
	Super::BeginPlay();

	if (ItemInfoDatabase == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[AVoxelGameState::BeginPlay]: ItemInfoDatabase not set. Check the blueprint and set the reference."));
	}
}
