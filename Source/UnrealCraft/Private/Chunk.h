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

private:
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
	 * @param Mask 
	 * @param AxisMask 
	 * @param V1 
	 * @param V2 
	 * @param V3 
	 * @param V4 
	 */
	void CreateQuad(FMask Mask, FIntVector AxisMask, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);
	
	/**
	 * Get the block type at the index.
	 * @remarks Contains checks for out of range.
	 */
	EBlock GetBlock(FIntVector Index) const;

	/**
	 * Get the 1D index for a block at a 3D index located in @link Blocks array.
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
};
