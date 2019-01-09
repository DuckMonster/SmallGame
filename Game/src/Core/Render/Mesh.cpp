
#include "Mesh.h"

void Mesh::Draw() const
{
	glBindVertexArray(vertex_buffer.vao);
	if (use_elements)
	{
		glDrawElements(draw_mode, draw_count, GL_UNSIGNED_INT, (void*)((size_t)draw_offset));
	}
	else
	{
		glDrawArrays(draw_mode, draw_offset, draw_count);
	}
	glBindVertexArray(0);
}
