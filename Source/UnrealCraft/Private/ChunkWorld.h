// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChunk.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

class FastNoise;
class ABaseChunk;

/**
 * Overarching manager for Chunks.
 */
UCLASS()
class AChunkWorld : public AActor
{
	GENERATED_BODY()

	TMap<FIntVector2, TObjectPtr<ABaseChunk>> Chunks;
public:
	UPROPERTY(EditAnywhere, Category="Chunk World")
	TSubclassOf<ABaseChunk> ChunksToSpawn = ABaseChunk::StaticClass(); 

	UPROPERTY(EditAnywhere, Category="Chunk World")
	int32 DrawDistanceAroundPlayer = 5;

	UPROPERTY(EditAnywhere, Category="Chunks")
	FIntVector NewChunkSize = FIntVector(32,32,32);
	
	UPROPERTY(EditInstanceOnly, Category="Chunks")
	TObjectPtr<UMaterialInterface> ChunkMaterial;

	static const FName DefaultChunkTag;
	
	TObjectPtr<FastNoise> NoiseGenerator;
	

	AChunkWorld();
	
	/**
	 * Get the noise object used to generate new chunks.
	 */
	TObjectPtr<FastNoise> GetNoise() const { return NoiseGenerator; }

protected:
	virtual void BeginPlay() override;

	/**
	 * Generate the initial chunks around the player.
	 */
	void GenerateInitialChunks();

	/**
	 * Configure the {@link NoiseGenerator}.
	 */
	void ConfigureNoiseGenerator();

public:	
	virtual void Tick(float DeltaTime) override;

	/**
	 * Get the adjacent chunk if possible.
	 * @param CurrentChunk The current chunk.
	 * @param Direction Which to check in. -x = Left, -y = Back.
	 * @param AdjacentChunk Returns the chunk, if found.
	 * @return True if an adjacent chunk is possible, false if not.
	 */
	bool GetAdjacentChunk(TObjectPtr<ABaseChunk> CurrentChunk, EDirection Direction, TObjectPtr<ABaseChunk>& AdjacentChunk);

};
