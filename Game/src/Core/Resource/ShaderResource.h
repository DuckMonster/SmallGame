#pragma once
#include "Resource.h"
#include <GL/glew.h>

class ShaderResource : public Resource
{
public:
	bool LoadInternal(const char* path) override;
	void UnloadInternal() override;

	GLuint handle;
	bool isValid = false;
};
