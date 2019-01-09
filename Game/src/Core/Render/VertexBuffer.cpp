
#include "VertexBuffer.h"

void VertexBuffer::Clear()
{
	glBindVertexArray(0);
}

void VertexBuffer::Create(int num_buffers /*= 0*/)
{
	Assert(!IsValid());

	glGenVertexArrays(1, &vao);
	glGenBuffers(num_buffers, buffers);
	this->num_buffers = num_buffers;
}

void VertexBuffer::Destroy()
{
	Assert(IsValid());

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(num_buffers, buffers);

	vao = GL_INVALID_INDEX;
	num_buffers = 0;
}

void VertexBuffer::AddBuffer(uint8& out_buffer)
{
	Assert(num_buffers < VB_MAX_BUFFERS);

	out_buffer = num_buffers;
	glGenBuffers(1, buffers + num_buffers);
}

void VertexBuffer::AddBuffers(uint8 num, uint8* out_buffers)
{
	Assert(num_buffers + num <= VB_MAX_BUFFERS);

	glGenBuffers(num, buffers + num_buffers);
	num_buffers += num;
}

void VertexBuffer::BufferData(uint8 index, void* data, uint32 size)
{
	Assert(index < num_buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[index]);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::BufferElementData(uint8 index, void* data, uint32 size)
{
	Assert(index < num_buffers);

	// Bind vertex array so the element buffer is remembered
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void VertexBuffer::BindBuffer(uint8 buffer_index, uint32 attrib_index, uint32 size, uint32 stride, uint32 offset)
{
	Assert(buffer_index < num_buffers);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer_index]);
	glEnableVertexAttribArray(attrib_index);
	glVertexAttribPointer(attrib_index, size, GL_FLOAT, false, sizeof(float) * stride, (void*)(sizeof(float) * offset));
	glBindVertexArray(0);
}

void VertexBuffer::Bind()
{
	Assert(IsValid());

	glBindVertexArray(vao);
}
