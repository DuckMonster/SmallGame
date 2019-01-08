#pragma once
#include <GL/glew.h>

#define VB_MAX_BUFFERS 4

struct VertexBuffer
{
	static void Clear();
	void Create(int numBuffers = 0);
	void Destroy();
	void AddBuffer(uint8& outBuffer);
	void AddBuffers(uint8 num, uint8* outBuffers);
	void BufferData(uint8 index, void* data, uint32 size);
	void BufferElementData(uint8 index, void* data, uint32 size);
	void BindBuffer(uint8 bufferIndex, uint32 attribIndex, uint32 size, uint32 stride, uint32 offset);
	void Bind();

	bool IsValid() const { return vao != GL_INVALID_INDEX; }

	GLuint vao = GL_INVALID_INDEX;
	GLuint buffers[VB_MAX_BUFFERS];
	uint8 numBuffers = 0;
};
