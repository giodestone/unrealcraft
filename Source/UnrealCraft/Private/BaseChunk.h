// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "BaseChunk.generated.h"

class UProceduralMeshComponent;
class FastNoise;

/// <summary>
///	Base class for all rendered chunks. 
/// </summary>
/// <remarks>
///	<seealso cref="ChunkWorld"/>
/// </remarks> 
UCLASS()
class UNREALCRAFT_API ABaseChunk : public AActor
{
	GENERATED_BODY()

protected:
	TObjectPtr<UProceduralMeshComponent> Mesh;
	TObjectPtr<FastNoise> NoiseGenerator;
	FIntVector ChunkSize;
	FIntVector2 ChunkLocation;
	
public:
	ABaseChunk();

	/**
	 * Called by {@link AChunkWorld} to initialise this.
	 */
	void Initialise(const TObjectPtr<FastNoise> InNoiseGen, const FIntVector InChunkSize, const FIntVector2 InChunkLocation);
	
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
