#pragma once
#include "VertexBuffer.h"

struct Mesh
{
	VertexBuffer vertex_buffer;
	uint32 draw_count;
	uint32 draw_offset;
	GLenum draw_mode;

	bool use_elements;
	void Draw() const;
};
