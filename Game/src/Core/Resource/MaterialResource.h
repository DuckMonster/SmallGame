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
	ShaderResource* vertex_resource = nullptr;
	ShaderResource* fragment_resource = nullptr;
	TextureResource* texture_resource = nullptr;

	Material material;
	bool is_valid = false;
};
