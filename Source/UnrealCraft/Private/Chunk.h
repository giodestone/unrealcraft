// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

enum class EBlock;
enum class EDirection;
class FastNoise;
class UProceduralMeshComponent;

UCLASS()
class AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();
	
	UPROPERTY(EditAnywhere, Category="Chunk")
	int Size = 32;

	UPROPERTY(EditAnywhere, Category="Chunk")
	int Scale = 1;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TObjectPtr<UProceduralMeshComponent> Mesh;
	TObjectPtr<FastNoise> Noise;

	TArray<EBlock> Blocks;

	TArray<FVector> VertexData;
	TArray<int> TriangleData;
	TArray<FVector2D> UVData;

	int VertexCount;

	// Lookup table for cube vertices.
	const FVector BlockVertexData[8] =
		{
			FVector(100, 100, 100),
			FVector(100, 0, 100),
			FVector(100, 0, 0),
			FVector(100,100,0),
			FVector(0,0,100),
			FVector(0, 100, 100),
			FVector(0, 100, 0),
			FVector(0,0,0)
		};

	// Indices for cube faces.
	const int BlockTriangleData[24] =
		{
			0,1,2,3, // Forward
			5,0,3,6, // Right
			4,5,6,7, // Back
			1,4,7,2, // Left
			5,4,1,0, // Up
			3,2,7,6  // Down
		};

	/// <summary> Populate blocks according to heightmap. </summary>
	void GenerateBlocks();

	/// <summary> Generates mesh from generated blocks. </summary>
	void GenerateMesh();

	/// <summary> Passes data onto <see cref="UProceduralMeshComponent"/> after mesh was generated. </summary>
	void ApplyMesh() const;

	/// <summary> Check whether position contains a block (<c>true</c>), or air (<c>false</c>). </summary>
	bool Check(const FVector& Position) const;

	/// <summary> Create a face for a block. </summary>
	void CreateFace(EDirection Direction, FVector Position);

	/// <summary> Gets the face vertices according to the lookup tables. </summary>
	TArray<FVector> GetFaceVertices(EDirection Direction, FVector Position) const;

	/// <summary> Get the coordinates of a vector in position. </summary>
	FVector GetPositionInDirection(EDirection Direction, FVector Position) const;
	
	/// <summary> Get the 1D index for a block at a 3D index located in <see cref="Blocks"/> array. </summary>
	int GetBlockIndex(const int X, const int Y, const int Z) const;

	/// <summary> Wrapper for <see cref="GetBlockIndex"/>. </summary>
	/// <remarks> Note this casts the vector components to <c>int</c>, potentially causing errors.</remarks>
	int GetBlockIndex(const FVector& Position) const;
};
