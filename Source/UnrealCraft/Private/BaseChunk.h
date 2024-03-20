// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "BaseChunk.generated.h"

class AChunkWorld;
class UProceduralMeshComponent;
class FastNoise;

/**
 * Base class for all rendered chunks. See also: {@link AChunk} 
 */
UCLASS() 
class UNREALCRAFT_API ABaseChunk : public AActor
{
	GENERATED_BODY()

protected:
	TObjectPtr<UProceduralMeshComponent> Mesh;
	TObjectPtr<FastNoise> NoiseGenerator;

	UPROPERTY(VisibleInstanceOnly, Category="Chunk")
	TObjectPtr<AChunkWorld> ChunkWorld;

	UPROPERTY(VisibleInstanceOnly, Category="Chunk")
	FIntVector ChunkSize;
	UPROPERTY(VisibleInstanceOnly, Category="Chunk")
	FIntVector2 ChunkLocation;

public:
	const FIntVector2& GetChunkLocation() const { return ChunkLocation; }
	const FIntVector& GetChunkSize() const { return ChunkSize; }
	TObjectPtr<AChunkWorld> GetChunkWorld() const { return ChunkWorld; }

	TObjectPtr<UMaterialInterface> Material;
	
	ABaseChunk();

	/**
	 * Called by {@link AChunkWorld} to initialise this.
	 */
	void Initialise(const TObjectPtr<FastNoise> InNoiseGen, const FIntVector InChunkSize, const FIntVector2 InChunkLocation, const TObjectPtr<AChunkWorld> InChunkWorld);

	/**
	 * Modify the block at the current position.
	 * @param Position The local position of the chunk. See {@link VoxelUtils @endlink}.
	 * @param NewBlock The new block at the location.
	 * @remarks Doesn't rebuild mesh. Appropriate methods must be called after this.
	 */
	virtual void ModifyVoxel(const FIntVector Position, const EBlock NewBlock);
protected:
	virtual void BeginPlay() override;

	/**
	 * For logic that should be performed after the chunk is initialised. Abstract.
	 */
	virtual void OnInitialiseComplete();

public:
	virtual void Tick(float DeltaTime) override;

private:

	/**
	 * Configures the {@link Mesh} component.
	 */
	void SetupMesh();
};
