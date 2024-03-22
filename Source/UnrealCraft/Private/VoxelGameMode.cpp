// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelGameMode.h"

#include "PlayerHUD.h"
#include "VoxelGameState.h"
#include "VoxelPawn.h"
#include "VoxelPlayerController.h"
#include "UnrealCraft/Public/PlayerInventory.h"

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

	auto VoxelGameState = GetGameState<AVoxelGameState>();

	// TODO: If this game ever goes multiplayer - this needs to be fixed.
	if (VoxelGameState != nullptr)
		VoxelGameState->GetInventoryDatabase().AddEntityInventory("Player", MakeShared<PlayerInventory>());

	// GLog->Logf(ELogVerbosity::Log, TEXT("[]: New Player Joined, name: %s"), NewPlayer->GetName().GetCharArray());
}
