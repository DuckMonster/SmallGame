
#include "VertexBuffer.h"

void VertexBuffer::Clear()
{
	glBindVertexArray(0);
}

void VertexBuffer::Create(int numBuffers /*= 0*/)
{
	Assert(!IsValid());

	glGenVertexArrays(1, &vao);
	glGenBuffers(numBuffers, buffers);
	this->numBuffers = numBuffers;
}

void VertexBuffer::Destroy()
{
	Assert(IsValid());

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(numBuffers, buffers);

	vao = GL_INVALID_INDEX;
	numBuffers = 0;
}

void VertexBuffer::AddBuffer(uint8& outBuffer)
{
	Assert(numBuffers < VB_MAX_BUFFERS);

	outBuffer = numBuffers;
	glGenBuffers(1, buffers + numBuffers);
}

void VertexBuffer::AddBuffers(uint8 num, uint8* outBuffers)
{
	Assert(numBuffers + num <= VB_MAX_BUFFERS);

	glGenBuffers(num, buffers + numBuffers);
	numBuffers += num;
}

void VertexBuffer::BufferData(uint8 index, void* data, uint32 size)
{
	Assert(index < numBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[index]);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::BufferElementData(uint8 index, void* data, uint32 size)
{
	Assert(index < numBuffers);

	// Bind vertex array so the element buffer is remembered
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void VertexBuffer::BindBuffer(uint8 bufferIndex, uint32 attribIndex, uint32 size, uint32 stride, uint32 offset)
{
	Assert(bufferIndex < numBuffers);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[bufferIndex]);
	glEnableVertexAttribArray(attribIndex);
	glVertexAttribPointer(attribIndex, size, GL_FLOAT, false, sizeof(float) * stride, (void*)(sizeof(float) * offset));
	glBindVertexArray(0);
}

void VertexBuffer::Bind()
{
	Assert(IsValid());

	glBindVertexArray(vao);
}
