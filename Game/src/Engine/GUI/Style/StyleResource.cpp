
#include "StyleResource.h"
#include "Core/Serialize/Json.h"
#include "Core/Resource/ResourceManager.h"

struct StyleTag
{
	const char* name;
	Style style;
};

// Overload for serializing StyleTag objects
template<>
bool JsonValue::Serialize<Style>(Style& style)
{
	bool result = true;
	result &= Serialize("foreground", style.foreground);
	result &= Serialize("background", style.background);

	return result;
}

template<>
bool JsonValue::Serialize<StyleTag>(StyleTag& tag)
{
	bool result = true;
	result &= Serialize("name", tag.name);
	result &= Serialize(tag.style);

	return result;
}

bool StyleResource::LoadInternal(const char* path)
{
	/* SERIALIZE FONT DATA */
	JsonDocument document;
	document.LoadFile(path);

	JsonValue object = document.GetRootObject();
	const char* parent_path = nullptr;
	const char* font_path = nullptr;
	Style default_style;
	TArray<StyleTag> tags;

	object.Serialize("parent", parent_path);
	object.Serialize("font", font_path);
	bool has_default = object.Serialize("default", default_style);

	object.SerializeArray("tags", tags);

	// Load parent
	StyleResource* parent = nullptr;
	if (parent_path != nullptr)
	{
		parent = gResourceManager->Load<StyleResource>(parent_path);
		if (parent != nullptr)
		{
			// Copy over palette from parent, before we write our own stuff
			palette.default_style = parent->palette.default_style;
			palette.tags = parent->palette.tags;
			font = parent->font;
		}
	}

	// Load font
	if (font_path != nullptr)
	{
		font = gResourceManager->Load<FontResource>(font_path);
	}

	if (font == nullptr)
	{
		Error("No font specified for style '%s'", path);
		return false;
	}

	AddDependency(font);
	if (parent != nullptr)
		AddDependency(parent);

	// Fill out the palette
	if (has_default)
		palette.default_style = default_style;

	for (StyleTag& tag : tags)
	{
		palette.tags[tag.name] = tag.style;
	}

	return true;
}

void StyleResource::UnloadInternal()
{
	ClearDependencies();
	palette.tags.Clear();
}
