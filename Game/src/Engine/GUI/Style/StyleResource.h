#pragma once
#include "Core/Resource/Resource.h"
#include "Style.h"
#include "Engine/GUI/Text/FontResource.h"

class StyleResource : public Resource
{
public:
	bool LoadInternal(const char* path) override;
	void UnloadInternal() override;

	FontResource* font;
	StylePalette palette;
};
