
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
	const char* parentPath = nullptr;
	const char* fontPath = nullptr;
	Style defaultStyle;
	TArray<StyleTag> tags;

	object.Serialize("parent", parentPath);
	object.Serialize("font", fontPath);
	bool hasDefault = object.Serialize("default", defaultStyle);

	object.SerializeArray("tags", tags);

	// Load parent
	StyleResource* parent = nullptr;
	if (parentPath != nullptr)
	{
		parent = gResourceManager->Load<StyleResource>(parentPath);
		if (parent != nullptr)
		{
			// Copy over palette from parent, before we write our own stuff
			palette.defaultStyle = parent->palette.defaultStyle;
			palette.tags = parent->palette.tags;
			font = parent->font;
		}
	}

	// Load font
	if (fontPath != nullptr)
	{
		font = gResourceManager->Load<FontResource>(fontPath);
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
	if (hasDefault)
		palette.defaultStyle = defaultStyle;

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
