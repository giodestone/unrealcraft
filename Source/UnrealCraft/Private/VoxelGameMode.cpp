// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelGameMode.h"

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
	
	check(GEngine != nullptr);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is VoxelGameMode!"));
}

void AVoxelGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto VoxelGameState = Cast<AVoxelGameState>(GameState);
	
	// TODO: If this game ever goes multiplayer - this needs to be fixed.
	
	if (VoxelGameState == nullptr)
		return;

	auto InitialItem = NewObject<UUnrealCraftItem>();
	InitialItem->Initialize("stone");
	
	auto CreatedPlayerInventory = MakeShared<PlayerInventory>();
	CreatedPlayerInventory->InsertAnywhere(InitialItem);
	VoxelGameState->GetInventoryDatabase().AddEntityInventory("Player", CreatedPlayerInventory);

	Cast<AVoxelGameState>(GameState)->SetPlayerInventory(CreatedPlayerInventory);

	// GLog->Logf(ELogVerbosity::Log, TEXT("[]: New Player Joined, name: %s"), NewPlayer->GetName().GetCharArray());
}
