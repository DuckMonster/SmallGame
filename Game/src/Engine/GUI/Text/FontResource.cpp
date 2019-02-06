
#include "FontResource.h"
#include "Core/Serialize/Json.h"
#include "Core/Resource/ResourceManager.h"

bool FontResource::LoadInternal(const char* path)
{
	/* SERIALIZE FONT DATA */
	JsonDocument document;
	document.LoadFile(path);

	JsonValue object = document.GetRootObject();
	const char* foreground_path = nullptr;
	const char* background_path = nullptr;
	const char* texture_path = nullptr;
	uint32 glyph_width = 0;
	uint32 glyph_height = 0;
	uint32 spacing_x = 0;
	uint32 spacing_y = 0;
	uint32 kerning;

	object.Serialize("material.foreground", foreground_path);
	object.Serialize("material.background", background_path);
	object.Serialize("texture", texture_path);
	object.Serialize("glyph.width", glyph_width);
	object.Serialize("glyph.height", glyph_height);
	object.Serialize("spacing.x", spacing_x);
	object.Serialize("spacing.y", spacing_y);
	object.Serialize("kerning", kerning);

	/* LOAD DEPENDENT RESOURCES */
	if (foreground_path != nullptr)
		foreground_material = resource_manager.Load<MaterialResource>(foreground_path);
	if (background_path != nullptr)
		background_material = resource_manager.Load<MaterialResource>(background_path);
	if (texture_path != nullptr)
		texture = resource_manager.Load<TextureResource>(texture_path);

	bool success = true;
	success |= Assert(foreground_material != nullptr);
	success |= Assert(background_material != nullptr);
	success |= Assert(texture != nullptr);

	if (!success)
	{
		return true;
	}

	AddDependency(foreground_material);
	AddDependency(background_material);
	AddDependency(texture);

	/* BUILD FONT */
	font = Font::Build(&texture->texture, glyph_width, glyph_height, spacing_x, spacing_y);
	return true;
}

void FontResource::UnloadInternal()
{
	ClearDependencies();
}
