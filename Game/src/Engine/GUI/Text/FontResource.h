#pragma once
#include "Core/Resource/Resource.h"
#include "Core/Resource/MaterialResource.h"
#include "Engine/GUI/Text/Font.h"

class FontResource : public Resource
{
public:
	bool LoadInternal(const char* path) override;
	void UnloadInternal() override;

	MaterialResource* foregroundMaterial;
	MaterialResource* backgroundMaterial;
	TextureResource* texture;

	Font font;
};
