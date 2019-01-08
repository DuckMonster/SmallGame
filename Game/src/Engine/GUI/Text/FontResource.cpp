
#include "FontResource.h"
#include "Core/Serialize/Json.h"
#include "Core/Resource/ResourceManager.h"

bool FontResource::LoadInternal(const char* path)
{
	/* SERIALIZE FONT DATA */
	JsonDocument document;
	document.LoadFile(path);

	JsonValue object = document.GetRootObject();
	const char* foregroundPath = nullptr;
	const char* backgroundPath = nullptr;
	const char* texturePath = nullptr;
	uint32 glyph_width = 0;
	uint32 glyph_height = 0;
	uint32 spacing_x = 0;
	uint32 spacing_y = 0;
	uint32 kerning;

	object.Serialize("material.foreground", foregroundPath);
	object.Serialize("material.background", backgroundPath);
	object.Serialize("texture", texturePath);
	object.Serialize("glyph.width", glyph_width);
	object.Serialize("glyph.height", glyph_height);
	object.Serialize("spacing.x", spacing_x);
	object.Serialize("spacing.y", spacing_y);
	object.Serialize("kerning", kerning);

	/* LOAD DEPENDENT RESOURCES */
	if (foregroundPath != nullptr)
		foregroundMaterial = gResourceManager->Load<MaterialResource>(foregroundPath);
	if (backgroundPath != nullptr)
		backgroundMaterial = gResourceManager->Load<MaterialResource>(backgroundPath);
	if (texturePath != nullptr)
		texture = gResourceManager->Load<TextureResource>(texturePath);

	bool success = true;
	success |= Assert(foregroundMaterial != nullptr);
	success |= Assert(backgroundMaterial != nullptr);
	success |= Assert(texture != nullptr);

	if (!success)
	{
		return true;
	}

	AddDependency(foregroundMaterial);
	AddDependency(backgroundMaterial);
	AddDependency(texture);

	/* BUILD FONT */
	font = Font::Build(&texture->texture, glyph_width, glyph_height, spacing_x, spacing_y);
	return true;
}

void FontResource::UnloadInternal()
{
	ClearDependencies();
}
