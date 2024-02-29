// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"


// Sets default values
AChunkWorld::AChunkWorld()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AChunkWorld::GenerateInitialChunks()
{
	for (auto x = -DrawDistance; x < DrawDistance; x++)
	{
		for (auto y = -DrawDistance; y < DrawDistance; y++)
		{
			GetWorld()->SpawnActor<AActor>(Chunk, FVector(x * ChunkSize * 100, y * ChunkSize * 100, 0), FRotator::ZeroRotator);
		}
	}
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	GenerateInitialChunks();
}

// Called every frame
void AChunkWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

