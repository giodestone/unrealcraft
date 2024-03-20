// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChunk.h"
#include "ChunkMeshData.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

enum class EBlock;
class FastNoise;
class UProceduralMeshComponent;

/**
 * Holds block data for a specific area of the world.
 */
UCLASS()
class AChunk : public ABaseChunk
{
	GENERATED_BODY()
	
	FChunkMeshData MeshData;
    TArray<EBlock> Blocks;

    int VertexCount = 0;

	struct FMask
	{
		EBlock Block;
		int Normal;
	};
	
public:	
	AChunk();

protected:
	virtual void BeginPlay() override;

	virtual void OnInitialiseComplete() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void ClearMeshData();

	virtual void ModifyVoxel(const FIntVector Position, const EBlock NewBlock) override;

	/**
	 * Check what block is at the coordinate.
	 * @remarks Returns air if out of bounds.
	 * @param Coord Where to check the for the block.
	 * @return The block at that coordinate
	 */
	virtual EBlock GetBlock(const FIntVector& Coord) const override;
private:
	
	void ModifyVoxelData(const FIntVector Position, EBlock NewBlock);
	
	/**
	 * Populate blocks according to heightmap.
	 */
	void GenerateBlocks();

	/**
	 * Generates mesh from generated blocks.
	 */
	void GenerateMesh();

	/**
	 * Passes data onto {@link UProceduralMeshComponent} after mesh was generated.
	 */
	void ApplyMesh() const;

	/**
	 * Creates vertices based on mask.
	 */
	void CreateQuad(FMask Mask, FIntVector AxisMask, const int Width, const int Height, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);

	/**
	 * Get the 1D index for a block at a 3D index located in @link Blocks array.
	 * @remarks Doesn't check if the coordinate is in bounds.
	 */
	int32 GetBlockIndex(const int32 X, const int32 Y, const int32 Z) const;
	
	/**
	 * Wrapper for @link GetBlockIndex.
	 * @remarks Casts floats to int, potentially resulting in rounding errors.
	 */
	int32 GetBlockIndex(const FVector& Position) const;

	/**
	 * Wrapper for @link GetBlockIndex. 
	 */
	int32 GetBlockIndex(const FIntVector& Position) const;

	/**
	 * Compare if two masks are identical.
	 */
	bool CompareMask(const FMask M1, const FMask M2) const;

	/**
	 * Get the texture index inside a Texture2DArray for a face.
	 * @param Block The block to get the details for.
	 * @param Normal The normal of the surface.
	 */
	int8 GetTextureIndex(EBlock Block, FVector Normal) const;

	/**
	 * Whether the coordinate is within the chunk bounds.
	 */
	bool IsInBounds(int32 X, int32 Y, int32 Z) const;

	/**
	 * Wrapper for {@link IsInBounds}.
	 */
	bool IsInBounds(FIntVector Coords) const;

	/**
	 * Get the direction of the overflow.
	 * @remark Will not check if the coordinate is out of bounds and will result in an invalid direction. Use {@link IsInBounds} to verify.
	 * @remark Will only check up to one adjacent chunk over in X or Y. *Not* in XY.
	 * @param Coord The overflowing coordinate.
	 * @param OutCoord The scaled coordinate.
	 * @return The equivalent direction.
	 */
	EDirection GetCoordinateOverflowDirection(const FIntVector& Coord, FIntVector& OutCoord) const;
};
