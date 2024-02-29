// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"

#include "BaseChunk.h"
#include "FastNoise.h"


// Sets default values
AChunkWorld::AChunkWorld()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AChunkWorld::GenerateInitialChunks()
{
	for (auto x = -DrawDistanceAroundPlayer; x < DrawDistanceAroundPlayer; x++)
	{
		for (auto y = -DrawDistanceAroundPlayer; y < DrawDistanceAroundPlayer; y++)
		{
			auto CreatedChunk = GetWorld()->SpawnActor<ABaseChunk>(
				ChunksToSpawn,
				FVector(static_cast<float>(x) * NewChunkSize.X * 100.f, static_cast<float>(y) * NewChunkSize.Y * 100.f, 0.f),
				FRotator::ZeroRotator);
			CreatedChunk->Initialise(NoiseGenerator, NewChunkSize, FIntVector2(x, y));
		}
	}
}

void AChunkWorld::ConfigureNoiseGenerator()
{
	NoiseGenerator = new FastNoise();
	NoiseGenerator->SetFrequency(0.03f);
	NoiseGenerator->SetNoiseType(FastNoise::Perlin);
	NoiseGenerator->SetFractalType(FastNoise::FractalType::FBM);
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	ConfigureNoiseGenerator();
	GenerateInitialChunks();
}

// Called every frame
void AChunkWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// disabled.
}

