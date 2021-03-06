#pragma once
#include <GL/glew.h>

struct Texture
{
	void Create();
	void Destroy();
	bool IsValid() const { return handle != GL_INVALID_INDEX; }

	void Bind(uint32 index = 0) const;

	GLuint handle = GL_INVALID_INDEX;
	int width = -1;
	int height = -1;
};