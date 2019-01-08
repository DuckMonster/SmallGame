
#include "Mesh.h"

void Mesh::Draw() const
{
	glBindVertexArray(vertexBuffer.vao);
	if (useElements)
	{
		glDrawElements(drawMode, drawCount, GL_UNSIGNED_INT, (void*)((size_t)drawOffset));
	}
	else
	{
		glDrawArrays(drawMode, drawOffset, drawCount);
	}
	glBindVertexArray(0);
}
