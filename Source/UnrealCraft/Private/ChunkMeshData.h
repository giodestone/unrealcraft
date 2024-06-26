﻿#pragma once

#include "CoreMinimal.h"
#include "ChunkMeshData.generated.h"

/**
 * For storing model data for a {@link AChunk}.
 */
USTRUCT()
struct FChunkMeshData
{
	GENERATED_BODY();

public:
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
	TArray<FColor> Colors;
	TArray<FVector2D> UV0;

	/**
	 * Reset the data.
	 */
	void Clear();
};
