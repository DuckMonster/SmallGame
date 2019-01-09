
#include "FrameBuffer.h"
#include "Core/Context/Context.h"

void FrameBuffer::Create(int width, int height)
{
	Assert(!IsValid());

	glCreateFramebuffers(1, &handle);
	this->width = width;
	this->height = height;
}

Texture& FrameBuffer::AddTexture()
{
	Assert(IsValid());

	Texture& texture = textures[num_textures];
	texture.Create();
	texture.width = width;
	texture.height = height;

	glBindTexture(GL_TEXTURE_2D, texture.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.handle, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	num_textures++;
	return texture;
}

Texture& FrameBuffer::AddDepthTexture()
{
	Assert(IsValid());

	Texture& texture = textures[num_textures];
	texture.Create();
	texture.width = width;
	texture.height = height;

	glBindTexture(GL_TEXTURE_2D, texture.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.handle, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	num_textures++;
	return texture;
}

Texture& FrameBuffer::AddDepthStencilTexture()
{
	Assert(IsValid());

	Texture& texture = textures[num_textures];
	texture.Create();
	texture.width = width;
	texture.height = height;

	glBindTexture(GL_TEXTURE_2D, texture.handle);
	glTexStorage2D(GL_TEXTURE_2D, 2, GL_DEPTH24_STENCIL8, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture.handle, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	num_textures++;
	return texture;
}

void FrameBuffer::Destroy()
{
	Assert(IsValid());

	glDeleteFramebuffers(1, &handle);
	glDeleteTextures(num_textures, (GLuint*)textures);
}

bool FrameBuffer::IsComplete()
{
	Assert(IsValid());

	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return status == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::Bind()
{
	Assert(IsValid());

	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glViewport(0, 0, width, height);
}

void FrameBuffer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, gContext->width, gContext->height);
}

