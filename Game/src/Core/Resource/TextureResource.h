#pragma once
#include "Resource.h"
#include "Core/Render/Texture.h"

class TextureResource : public Resource
{
public:
	bool LoadInternal(const char* path) override;
	void UnloadInternal() override;

	Texture texture;
};
