// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelGameMode.h"

#include "VoxelPawn.h"
#include "VoxelPlayerController.h"

AVoxelGameMode::AVoxelGameMode()
{
	this->PlayerControllerClass = AVoxelPlayerController::StaticClass();
	this->DefaultPawnClass = AVoxelPawn::StaticClass();
}

void AVoxelGameMode::StartPlay()
{
	Super::StartPlay();
	
	check(GEngine != nullptr);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is VoxelGameMode!"));
}
