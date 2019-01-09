#pragma once
#include <GL/glew.h>

#define VB_MAX_BUFFERS 4

struct VertexBuffer
{
	static void Clear();
	void Create(int num_buffers = 0);
	void Destroy();
	void AddBuffer(uint8& out_buffer);
	void AddBuffers(uint8 num, uint8* out_buffers);
	void BufferData(uint8 index, void* data, uint32 size);
	void BufferElementData(uint8 index, void* data, uint32 size);
	void BindBuffer(uint8 buffer_index, uint32 attrib_index, uint32 size, uint32 stride, uint32 offset);
	void Bind();

	bool IsValid() const { return vao != GL_INVALID_INDEX; }

	GLuint vao = GL_INVALID_INDEX;
	GLuint buffers[VB_MAX_BUFFERS];
	uint8 num_buffers = 0;
};
