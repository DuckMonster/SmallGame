#pragma once
#include "Core/Standard/Map.h"

struct Style
{
	Color foreground;
	Color background;
};

class StylePalette
{
public:
	Style default_style;
	Map<String, Style> tags;

	const Style& Get(const char* tag) const
	{
		// Does the tag exist?
		const Style* style = tags.Find(tag);
		if (style == nullptr)
			return default_style;

		return *style;
	}
};
