#pragma once
#include <GL/glew.h>
#include "Texture.h"

#define FRAMEBUFFER_MAX_TEXTURES 4

struct FrameBuffer
{
	void Create(int width, int height);

	Texture& AddTexture();
	Texture& AddDepthTexture();
	Texture& AddDepthStencilTexture();

	void Destroy();
	bool IsComplete();
	void Bind();
	static void Clear();

	bool IsValid() const { return handle != GL_INVALID_INDEX; }

	GLuint handle = GL_INVALID_INDEX;
	Texture textures[FRAMEBUFFER_MAX_TEXTURES];
	uint32 numTextures;

	int width;
	int height;
};