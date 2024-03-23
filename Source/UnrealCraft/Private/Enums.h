#pragma once

#include "CoreMinimal.h"

/**
 * Enum wrapper around directions.
 * @remarks Order is important for {@link UChunk::GetFaceVertices} to work correctly.
 */
enum class EDirection : int32
{
	// 
	Forward, Right, Back, Left, Up, Down
};

/**
 * Enum for all block types.
 */
UENUM()
enum class EBlock
{
	Null, Air, Stone, Dirt, Grass, Planks, Inventory
};