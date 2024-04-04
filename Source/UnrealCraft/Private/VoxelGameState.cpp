// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelGameState.h"

#include "InventoryDatabase.h"
#include "PlayerInventory.h"
#include "InventoryVisualizerWidget.h"

UInventoryDatabase& AVoxelGameState::GetInventoryDatabase()
{
	if (MainInventoryDatabase == nullptr)
	{
		MainInventoryDatabase = NewObject<UInventoryDatabase>();
		MainInventoryDatabase->Initialize();
	}
	
	return *MainInventoryDatabase;
}

TObjectPtr<UInventoryVisualizerWidget> AVoxelGameState::GetInventoryVisualizer() const
{ return InventoryVisualizer; }

TObjectPtr<UPlayerInventory> AVoxelGameState::GetPlayerInventory() const
{
	return MainPlayerInventory;
}

void AVoxelGameState::SetInventoryVisualizer(TObjectPtr<UInventoryVisualizerWidget> NewInventoryVisualizer)
{
	InventoryVisualizer = NewInventoryVisualizer;
}

void AVoxelGameState::SetPlayerInventory(TObjectPtr<UPlayerInventory> NewPlayerInventory)
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
