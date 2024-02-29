// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

class AChunk;

UCLASS()
class AChunkWorld : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AChunkWorld();

	// TODO: Create base class for chunk
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	TSubclassOf<AActor> Chunk = AChunk::StaticClass(); 

	UPROPERTY(EditAnywhere, Category="Chunk World")
	int DrawDistance = 5;

	UPROPERTY(EditAnywhere, Category="Chunk World")
	int ChunkSize = 32;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateInitialChunks();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
