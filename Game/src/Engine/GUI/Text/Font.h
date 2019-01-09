#pragma once
#include "Core/Render/Texture.h"

// A specific glyph for a character in some font
struct Glyph
{
	Vec2 pixel_size;
	Vec2 uv_offset;
	Vec2 uv_size;
};

struct Font
{
	// Builds font data
	static Font Build(
		Texture* texture,
		int glyph_width, int glyph_height,
		int spacing_x, int spacing_y)
	{
		Font font;

		font.pixel_size = Vec2(glyph_width, glyph_height);
		font.pixel_spacing = Vec2(spacing_x, spacing_y);

		font.uv_size = Vec2(
			(float)glyph_width / texture->width,
			(float)glyph_height / texture->height
		);
		font.uv_size.y *= -1.f;
		font.uv_spacing = Vec2(
			(float)spacing_x / texture->width,
			(float)spacing_y / texture->height
		);
		font.uv_spacing.y *= -1.f;

		font.glyphs_per_row = (texture->width / (glyph_width + spacing_x));

		return font;
	}

	// Gets a glyph on the specified x and y coordinate
	void BuildGlyphAt(Glyph& out_glyph, int x, int y) const
	{
		out_glyph.pixel_size = pixel_size;
		out_glyph.uv_size = uv_size;
		out_glyph.uv_offset = Vec2(
			(uv_size.x + uv_spacing.x) * x,
			1.f + (uv_size.y + uv_spacing.y) * y
		);
	}

	// Calculate and find the glyph for at specific character
	void BuildGlyphForChar(Glyph& out_glyph, char c) const
	{
		int x = (int)c % glyphs_per_row;
		int y = (int)(c / glyphs_per_row);

		BuildGlyphAt(out_glyph, x, y);
	}

	// Build glyphs for a string
	uint32 BuildGlyphsForString(const char* str, Glyph* glyph_array, uint32 array_num) const
	{
		uint32 num = Math::Min<uint32>(strlen(str), array_num);
		for (uint32 i = 0; i < num; ++i)
		{
			BuildGlyphForChar(glyph_array[i], str[i]);
		}

		return num;
	}

	Texture* texture;

	Vec2 pixel_size;
	Vec2 pixel_spacing;
	Vec2 uv_size;
	Vec2 uv_spacing;
	int glyphs_per_row;
};
