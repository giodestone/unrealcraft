// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

#include "Enums.h"
#include "ProceduralMeshComponent.h"
#include "FastNoise.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AChunk::OnInitialiseComplete()
{
	Blocks.SetNum(ChunkSize.X * ChunkSize.Y * ChunkSize.Z);
	
	GenerateBlocks();
    GenerateMesh();
    ApplyMesh();
}

void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// disabled.
}

void AChunk::ClearMeshData()
{
	VertexCount = 0;
	MeshData.Clear();
}

void AChunk::ModifyVoxel(const FIntVector Position, const EBlock NewBlock)
{
	if (Position.X >= ChunkSize.X || Position.Y >= ChunkSize.Y || Position.Z >= ChunkSize.Z ||
		Position.X < 0 || Position.Y < 0 || Position.Z < 0)
		return;

	ModifyVoxelData(Position, NewBlock);

	ClearMeshData();
	
	GenerateMesh();

	ApplyMesh();
}

void AChunk::ModifyVoxelData(const FIntVector Position, EBlock NewBlock)
{
	// TODO: Add adjacent chunk placing. Basically find out where is the chunk out of bounds, get it, and then modify
	// said voxel there. IsInBounds is the beginning point.
	
	const int32 Index = GetBlockIndex(Position);

	Blocks[Index] = NewBlock;
}

void AChunk::GenerateBlocks()
{
	const auto Location = GetActorLocation();

    for (auto x = 0.f; x < static_cast<float>(ChunkSize.X); x += 1.f)
    {
    	for (auto y = 0.f; y < static_cast<float>(ChunkSize.Y); y += 1.f)
    	{
    		const auto XPos = (x * 100.f + static_cast<float>(Location.X)) / 100.f;
    		const auto YPos = (y * 100.f + static_cast<float>(Location.Y)) / 100.f;

    		const auto Height = FMath::Clamp(FMath::RoundToInt((NoiseGenerator->GetNoise(XPos, YPos) + 1.f) * static_cast<float>(ChunkSize.Z) / 2.f), 0, static_cast<float>(ChunkSize.Z));

    		// TODO: Consider rewriting to use int in the first instance.
    		
    		for (auto z = 0.f; z < static_cast<float>(ChunkSize.Z); z += 1.f)
    		{
    			// top block of height should be grass
    			// four blocks below should be dirt
    			// everything below should be stone
    			if (z > Height)
    				Blocks[GetBlockIndex(x,y,z)] = EBlock::Air;
    			else if (z == Height)
    				Blocks[GetBlockIndex(x,y,z)] = EBlock::Grass;
    			else if (z < Height && z > Height - 4)
    				Blocks[GetBlockIndex(x,y,z)] = EBlock::Dirt;
    			else
    				Blocks[GetBlockIndex(x,y,z)] = EBlock::Stone;
			    //
    			//
    			// // Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
    			// if (z < Height - 3)
    			// 	Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
    			// else if (z < Height - 1)
    			// 	Blocks[GetBlockIndex(x, y, z)] = EBlock::Dirt;
    			// else if (z == Height - 1)
    			// 	Blocks[GetBlockIndex(x, y, z)] = EBlock::Grass;
    			// else
    			// 	Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
    		}

    		// for (auto z = Height; z < static_cast<float>(ChunkSize.Z); z += 1.f)
    		// {
    		// 	Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
    		// }
    	}
    }
}

void AChunk::GenerateMesh()
{
	// Axis = plane. e.g. an XY plane.
	
	for (auto Axis = 0; Axis < 3; Axis++)
	{
		// Perpendicular axes.
		const auto Axis1 = (Axis + 1) % 3; 
		const auto Axis2 = (Axis + 2) % 3;

		const auto MainAxisLimit = ChunkSize[Axis];
		const auto Axis1Limit = ChunkSize[Axis1];
		const auto Axis2Limit = ChunkSize[Axis2];

		// Growth in particular direction.
		auto DeltaAxis1 = FIntVector::ZeroValue;
		auto DeltaAxis2 = FIntVector::ZeroValue;

		// Represents blocks that are being iterated thru.
		auto ChunkIteration = FIntVector::ZeroValue;
		// Represents which mask is active.
		auto AxisMask = FIntVector::ZeroValue;

		AxisMask[Axis] = 1;

		TArray<FMask> Mask; // 2D array

		Mask.SetNum(Axis1Limit * Axis2Limit);

		// Check each slice.
		for (ChunkIteration[Axis] = -1; ChunkIteration[Axis] < MainAxisLimit; )
		{
			auto N = 0; // Index inside of the mask.

			for (ChunkIteration[Axis2] = 0; ChunkIteration[Axis2] < Axis2Limit; ChunkIteration[Axis2]++)
			{
				for (ChunkIteration[Axis1] = 0; ChunkIteration[Axis1] < Axis1Limit; ChunkIteration[Axis1]++)
				{
					const auto CurrentBlock = GetBlock(ChunkIteration);
					const auto CompareBlock = GetBlock(ChunkIteration + AxisMask); // Adjacent block.

					const auto IsCurrentBlockOpaque = CurrentBlock != EBlock::Air;
					const auto IsCompareBlockOpaque = CompareBlock != EBlock::Air;

					if (IsCurrentBlockOpaque == IsCompareBlockOpaque)
					{
						Mask[N++] = FMask { EBlock::Null, 0 };
					}
					else if (IsCurrentBlockOpaque)
					{
						Mask[N++] = FMask { CurrentBlock, 1 }; // If towards compare block, normal is 1.
					}
					else
					{
						Mask[N++] = FMask {CompareBlock, -1};
					}
				}
			}

			ChunkIteration[Axis]++;
			N = 0;

			// Generate mesh from the mask.

			for (auto j = 0; j < Axis2Limit; j++)
			{
				for (auto i = 0; i < Axis1Limit; )
				{
					if (Mask[N].Normal == 0)
					{
						i++;
						N++;
						continue;
					}

					const auto CurrentMask = Mask[N];
					ChunkIteration[Axis1] = i;
					ChunkIteration[Axis2] = j;

					int Width;

					for (Width = 1; i + Width < Axis1Limit && CompareMask(Mask[N + Width], CurrentMask); Width++)
					{
						
					}

					int Height;
					auto IsDone = false;

					for (Height = 1; j + Height < Axis2Limit; Height++)
					{
						for (auto k = 0; k < Width; k++)
						{
							if (CompareMask(Mask[N + k + Height * Axis1Limit], CurrentMask))
								continue;

							IsDone = true;
							break;
						}

						if (IsDone)
							break;
					}

					DeltaAxis1[Axis1] = Width;
					DeltaAxis2[Axis2] = Height;

					CreateQuad(CurrentMask, AxisMask, Width, Height,
						ChunkIteration,
						ChunkIteration + DeltaAxis1,
						ChunkIteration + DeltaAxis2,
						ChunkIteration + DeltaAxis1 + DeltaAxis2);

					// Reset for next iteration.
					DeltaAxis1 = FIntVector::ZeroValue;
					DeltaAxis2 = FIntVector::ZeroValue;

					for (auto l = 0; l < Height; l++)
					{
						for (auto k = 0; k < Width; k++)
						{
							Mask[N + k + l * Axis1Limit] = FMask { EBlock::Null, 0 };
						}
					}

					// Increment for next iteration.
					i += Width;
					N += Width;
				}
			}
		}
	}
}

void AChunk::ApplyMesh() const
{
	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.Colors, TArray<FProcMeshTangent>(), true);
}

void AChunk::CreateQuad(FMask Mask, FIntVector AxisMask, const int Width, const int Height, FIntVector V1, FIntVector V2, FIntVector V3,
	FIntVector V4)
{
	const auto Normal = FVector(AxisMask * Mask.Normal);

	MeshData.Vertices.Add(FVector(V1) * 100);
	MeshData.Vertices.Add(FVector(V2) * 100);
	MeshData.Vertices.Add(FVector(V3) * 100);
	MeshData.Vertices.Add(FVector(V4) * 100);

	MeshData.Triangles.Add(VertexCount);
	MeshData.Triangles.Add(VertexCount + 2 + Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 2 - Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 3);
	MeshData.Triangles.Add(VertexCount + 1 - Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 1 + Mask.Normal);

	if (Normal.X == 1 || Normal.X == -1)
	{
		MeshData.UV0.Append({
			FVector2D(Width, Height),
			FVector2D(0, Height),
			FVector2D(Width, 0),
			FVector2D(0, 0),
		});
	}
	else
	{
		MeshData.UV0.Append({
			FVector2D(Height, Width),
			FVector2D(Height, 0),
			FVector2D(0, Width),
			FVector2D(0, 0),
		});
	}

	auto Color = FColor(0, 0, 0, GetTextureIndex(Mask.Block, Normal));
	MeshData.Colors.Append({ Color, Color, Color, Color });

	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);

	VertexCount += 4;
}

EBlock AChunk::GetBlock(FIntVector Index) const
{
	if (Index.X >= ChunkSize.X || Index.Y >= ChunkSize.Y || Index.Z >= ChunkSize.Z ||
		Index.X < 0 || Index.Y < 0 || Index.Z < 0)
		return EBlock::Air;

	return Blocks[GetBlockIndex(Index)];
}

int32 AChunk::GetBlockIndex(const int32 X, const int32 Y, const int32 Z) const
{
	return Z * ChunkSize.X * ChunkSize.Y + Y * ChunkSize.X + X;
}

int32 AChunk::GetBlockIndex(const FVector& Position) const
{
	return GetBlockIndex(static_cast<int>(Position.X), static_cast<int>(Position.Y), static_cast<int>(Position.Z));
}

int32 AChunk::GetBlockIndex(const FIntVector& Position) const
{
	return GetBlockIndex(Position.X, Position.Y, Position.Z);
}

bool AChunk::CompareMask(const FMask M1, const FMask M2) const
{
	return M1.Block == M2.Block && M1.Normal == M2.Normal;
}

int8 AChunk::GetTextureIndex(EBlock Block, FVector Normal) const
{
	switch (Block)
	{
	case EBlock::Grass:
		if (Normal == FVector::UpVector) // TODO: Fix floating point precision.
			return 0;
		if (Normal == FVector::DownVector)
			return 2;
		return 1;
		
	case EBlock::Dirt:
		return 2;
		
	case EBlock::Stone:
		return 3;
		
	default:
#if UE_BUILD_DEVELOPMENT
			UE_LOG(LogTemp,	Warning, TEXT("[Chunk::GetTextureIndex]: Unset value for texture index. Returning 0."));
#endif
		return 0;
	}
}

bool AChunk::IsInBounds(int32 X, int32 Y, int32 Z) const
{
	return X >= 0 && Y >= 0 && Z >= 0 &&
		X < ChunkSize.X && Y < ChunkSize.Y && ChunkSize.Z < Z;
}

bool AChunk::IsInBounds(FIntVector Coords) const
{
	return IsInBounds(Coords.X, Coords.Y, Coords.Z);
}

