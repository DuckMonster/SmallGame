#pragma once
#include "Resource.h"
#include "Core/Render/Material.h"
#include "ShaderResource.h"
#include "TextureResource.h"

class MaterialResource : public Resource
{
public:
	bool LoadInternal(const char* path) override;
	void UnloadInternal() override;

	MaterialResource* parent = nullptr;
	ShaderResource* vertexResource = nullptr;
	ShaderResource* fragmentResource = nullptr;
	TextureResource* textureResource = nullptr;

	Material material;
	bool isValid = false;
};
