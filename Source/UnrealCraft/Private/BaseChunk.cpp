// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChunk.h"

#include "ProceduralMeshComponent.h"
#include "FastNoise.h"

ABaseChunk::ABaseChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupMesh();
}

void ABaseChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// disabled
}

void ABaseChunk::SetupMesh()
{
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Mesh->SetCastShadow(false);
	SetRootComponent(Mesh);
}

void ABaseChunk::Initialise(const TObjectPtr<FastNoise> InNoiseGen, const FIntVector InChunkSize, const FIntVector2 InChunkLocation, const TObjectPtr<AChunkWorld> InChunkWorld)
{
	this->NoiseGenerator = InNoiseGen;
	this->ChunkSize = InChunkSize;
	this->ChunkLocation = InChunkLocation;
	this->ChunkWorld = InChunkWorld;

	OnInitialiseComplete();
}

void ABaseChunk::ModifyVoxel(const FIntVector Position, const EBlock NewBlock)
{
}

EBlock ABaseChunk::GetBlock(const FIntVector& Coord) const
{
	return EBlock::Null;
}

void ABaseChunk::OnInitialiseComplete()
{
}

