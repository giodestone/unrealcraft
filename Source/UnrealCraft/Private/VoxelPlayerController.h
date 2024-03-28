// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VoxelPlayerController.generated.h"

/**
 * Override for a voxel specific player controller.
 */
UCLASS()
class AVoxelPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVoxelPlayerController();
};
