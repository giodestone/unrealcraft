#pragma once

/**
 * Contains function to help with dealing with blocks/voxels in the world.
 */
class VoxelUtils
{
public:
	static FIntVector WorldToBlockPosition(const FVector& WorldPosition)
	{
		return FIntVector(WorldPosition / 100.f);
	}

	static FIntVector WorldToLocalBlockPosition(const FVector& Position, const FIntVector& ChunkSize)
	{
		const auto ChunkPosition = WorldToChunkPosition(Position, ChunkSize);

		auto Result = WorldToBlockPosition(Position) - ChunkPosition * ChunkSize;

		if (ChunkPosition.X < 0)
			Result.X -= 1;
		if (ChunkPosition.Y < 0)
			Result.Y -= 1;
		if (ChunkPosition.Z < 0)
			Result.Z -= 1;

		return Result;
	}

	static FIntVector WorldToChunkPosition(const FVector& Position, const FIntVector& ChunkSize)
	{
		FIntVector Result;

		const FIntVector Factor = ChunkSize * 100.f;

		const auto IntPos = FIntVector(Position);

		if (IntPos.X < 0)
			Result.X = static_cast<int32>(Position.X / Factor.X) - 1;
		else
			Result.X = static_cast<int32>(Position.X / Factor.X);
		
		if (IntPos.Y < 0)
			Result.Y = static_cast<int32>(Position.Y / Factor.Y) - 1;
		else
			Result.Y = static_cast<int32>(Position.Y / Factor.Y);

		if (IntPos.Z < 0)
			Result.Z = static_cast<int32>(Position.Z / Factor.Z) - 1;
		else
			Result.Z = static_cast<int32>(Position.Z / Factor.Z);
		
		return Result;
	}
};
