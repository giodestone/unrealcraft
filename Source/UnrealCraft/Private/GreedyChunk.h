// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkMeshData.h"
#include "GameFramework/Actor.h"
#include "GreedyChunk.generated.h"

enum class EBlock;
class FastNoise;
class UProceduralMeshComponent;

UCLASS()
class AGreedyChunk : public AActor
{
	GENERATED_BODY()

	struct FMask
	{
		EBlock Block;
		int Normal;
	};
	
public:	
	// Sets default values for this actor's properties
	AGreedyChunk();

	UPROPERTY(EditAnywhere, Category="Chunk")
	FIntVector Size = FIntVector(1,1,1) * 32;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TObjectPtr<UProceduralMeshComponent> Mesh;
	TObjectPtr<FastNoise> Noise;

	FChunkMeshData MeshData;
	TArray<EBlock> Blocks;

	int VertexCount = 0;

	/// <summary> Populate blocks according to heightmap. </summary>
	void GenerateBlocks();

	/// <summary> Generates mesh from generated blocks. </summary>
	void GenerateMesh();

	/// <summary> Passes data onto <see cref="UProceduralMeshComponent"/> after mesh was generated. </summary>
	void ApplyMesh() const;

	/// <summary> Creates vertices based on mask. </summary>
	void CreateQuad(FMask Mask, FIntVector AxisMask, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);

	/// <summary> Get the block type at the index. </summary>
	/// <remarks> Contains checks for out of range. </remarks>
	EBlock GetBlock(FIntVector Index) const;
	
	/// <summary> Get the 1D index for a block at a 3D index located in <see cref="Blocks"/> array. </summary>
	int GetBlockIndex(const int X, const int Y, const int Z) const;

	/// <summary> Wrapper for <see cref="GetBlockIndex"/>. </summary>
	/// <remarks> Note this casts the vector components to <c>int</c>, potentially causing errors.</remarks>
	int GetBlockIndex(const FVector& Position) const;

	/// <summary Wrapper for <see cref="GetBlockIndex"/>. </summary>
	int GetBlockIndex(const FIntVector& Position) const;

	/// <summary> Compare if two mask values are identical. </summary>
	bool CompareMask(const FMask M1, const FMask M2) const;
	
};
