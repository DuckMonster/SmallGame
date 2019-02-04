#pragma once
#include "Resource.h"
#include <GL/glew.h>
#include <Core/Standard/Map.h>

struct ShaderParameter
{
	TString name;
	TString value;
};

class ShaderResource : public Resource
{
public:
	bool LoadInternal(const char* path) override;
	void UnloadInternal() override;

	GLuint Compile(const Map<String, String>& parameters);

	String source;
};
