#pragma once
#include "VertexBuffer.h"

struct Mesh
{
	VertexBuffer vertexBuffer;
	uint32 drawCount;
	uint32 drawOffset;
	GLenum drawMode;

	bool useElements;
	void Draw() const;
};
