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

public:
	UPROPERTY(EditAnywhere, Category="Chunk World")
	TSubclassOf<ABaseChunk> ChunksToSpawn = ABaseChunk::StaticClass(); 

	UPROPERTY(EditAnywhere, Category="Chunk World")
	int32 DrawDistanceAroundPlayer = 5;

	UPROPERTY(EditAnywhere, Category="Chunks")
	FIntVector NewChunkSize = FIntVector(32,32,128);
	
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

};
