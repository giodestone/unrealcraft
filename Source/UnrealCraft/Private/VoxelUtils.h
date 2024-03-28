#pragma once

/**
 * Contains function to help with dealing with blocks/voxels in the world.
 */
class VoxelUtils
{
public:
	/**
	 * Convert a world to the global position of a block.
	 * @param WorldPosition The world position.
	 */
	static FIntVector WorldToBlockPosition(const FVector& WorldPosition)
	{
		return FIntVector(WorldPosition / 100.f);
	}

	/**
	 * Convert a world position to a local block position in relation to a chunk size.
	 * @param Position World coordinate.
	 * @param ChunkSize The size of the target chunk.
	 * @remark The returned block coordinate may be larger/smaller than the chunk size.
	 */
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

	/**
	 * Convert a world position to a chunk position.
	 * @param Position The world position.
	 * @param ChunkSize The size of each chunk.
	 * @remark The resulting position may be greater than the chunks in the world.
	 * @remark This function assumes all chunks are the same size.
	 */
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
