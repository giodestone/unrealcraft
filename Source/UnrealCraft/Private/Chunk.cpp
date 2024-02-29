// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

#include "Enums.h"
#include "ProceduralMeshComponent.h"
#include "FastNoise.h"
#include "Logging/StructuredLog.h"

// Sets default values
AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = new FastNoise();
	Noise->SetFrequency(0.03f);
	Noise->SetNoiseType(FastNoise::Perlin);
	Noise->SetFractalType(FastNoise::FractalType::FBM);

	Blocks.SetNum(pow(static_cast<float>(Size), 3.f));

	Mesh->SetCastShadow(false);
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	GenerateBlocks();
	GenerateMesh();
	ApplyMesh();
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// disabled.
}

void AChunk::GenerateBlocks()
{
	const auto Location = GetActorLocation();

	const auto SizeFloat = static_cast<float>(Size);

	for (auto x = 0.f; x < SizeFloat; x += 1.f)
	{
		for (auto y = 0.f; y < SizeFloat; y += 1.f)
		{
			const auto XPos = (x * 100.f + static_cast<float>(Location.X)) / 100.f;
			const auto YPos = (y * 100.f + static_cast<float>(Location.Y)) / 100.f;

			const auto Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(XPos, YPos) + 1.f) * SizeFloat / 2.f), 0, SizeFloat);

			// TODO: Consider rewriting to use int in the first instance.
			
			for (auto z = 0.f; z < Height; z += 1.f)
			{
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
			}

			for (auto z = Height; z < SizeFloat; z += 1.f)
			{
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
			}
		}
	}
}

void AChunk::GenerateMesh()
{
	for (auto x = 0; x < Size; x++)
	{
		for (auto y = 0; y < Size; y++)
		{
			for (auto z = 0; z < Size; z++)
			{
				if (Blocks[GetBlockIndex(x, y, z)] == EBlock::Air)
					continue;

				const auto Position = FVector(x,y,z);

				// This calculates where the face(s) should be drawn by checking what's next to it.
				// Directions MUST be specified in order they are declared in. See EDirection for more info.
				for (auto Direction : { EDirection::Forward, EDirection::Right, EDirection::Back, EDirection::Left, EDirection::Up, EDirection::Down })
				{
					if (Check(GetPositionInDirection(Direction, Position)))
					{
						CreateFace(Direction, Position * 100);
					}
				}
			}
		}
	}
}

void AChunk::ApplyMesh() const
{
	Mesh->CreateMeshSection(0, VertexData, TriangleData, TArray<FVector>(), UVData, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

bool AChunk::Check(const FVector& Position) const
{
	// if out of bounds
	if (static_cast<int>(Position.X) >= Size || static_cast<int>(Position.Y) >= Size || static_cast<int>(Position.Z) >= Size ||
		Position.X < 0.f || Position.Y < 0.f || Position.Z < 0.f)
		return true;

	return Blocks[GetBlockIndex(Position)] == EBlock::Air;
}

void AChunk::CreateFace(EDirection Direction, FVector Position)
{
	VertexData.Append(GetFaceVertices(Direction, Position));
	UVData.Append({ FVector2D(1,1), FVector2D(1, 0), FVector2D(0,0), FVector2D(0, 1)});
	TriangleData.Append({
		VertexCount + 3, VertexCount + 2, VertexCount,
		VertexCount + 2, VertexCount + 1, VertexCount });
	VertexCount += 4;
}

TArray<FVector> AChunk::GetFaceVertices(EDirection Direction, FVector Position) const
{
	TArray<FVector> FaceVertices;

	for (auto i = 0; i < 4; i++)
	{
		FaceVertices.Add(BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]] * Scale + Position);
	}

	return FaceVertices;
}

FVector AChunk::GetPositionInDirection(EDirection Direction, FVector Position) const
{
	switch (Direction)
	{
	case EDirection::Forward:
		return Position + FVector::ForwardVector;
	case EDirection::Right:
		return Position + FVector::RightVector;
	case EDirection::Back:
		return Position + FVector::BackwardVector;
	case EDirection::Down:
		return Position + FVector::DownVector;
	case EDirection::Left:
		return Position + FVector::LeftVector;
	case EDirection::Up:
		return Position + FVector::UpVector;
	default:
		UE_LOGFMT(LogTemp, Error,
			"{Class}: Invalid Argument. Undefined behaviour for direction '{Direction}.",
			("Class", __FUNCTION__),
			("Direction", static_cast<int>(Direction)));
		return Position;
	}
}

int AChunk::GetBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

int AChunk::GetBlockIndex(const FVector& Position) const
{
	return GetBlockIndex(static_cast<int>(Position.X), static_cast<int>(Position.Y), static_cast<int>(Position.Z));
}

