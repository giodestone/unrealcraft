// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VoxelGameMode.generated.h"

class IInventoryInterface;
/**
 * Overrides for the voxel game mode.
 */
UCLASS()
class AVoxelGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	 AVoxelGameMode();
	
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	/**
	 * Adds a full stack of all possible items inside of UItemInventoryDatabase.
	 */
	void AddInitialItems(IInventoryInterface* TargetInventory);
};
