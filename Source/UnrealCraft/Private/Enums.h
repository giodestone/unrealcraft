#pragma once

enum class EDirection
{
	// Order is important for UChunk::GetFaceVertices to work correctly. Related to UChunk::BlockTriangleData order too.
	Forward, Right, Back, Left, Up, Down
};

enum class EBlock
{
	Null, Air, Stone, Dirt, Grass
};