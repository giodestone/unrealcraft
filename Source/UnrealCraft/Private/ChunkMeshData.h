#pragma once

#include "CoreMinimal.h"
#include "ChunkMeshData.generated.h"

/// <summary> For storing a chunks' mesh data. </summary>
USTRUCT()
struct FChunkMeshData
{
	GENERATED_BODY();

public:
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
};
