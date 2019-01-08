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
		int glyphWidth, int glyphHeight,
		int spacingX, int spacingY)
	{
		Font font;

		font.pixel_size = Vec2(glyphWidth, glyphHeight);
		font.pixel_spacing = Vec2(spacingX, spacingY);

		font.uv_size = Vec2(
			(float)glyphWidth / texture->width,
			(float)glyphHeight / texture->height
		);
		font.uv_size.y *= -1.f;
		font.uv_spacing = Vec2(
			(float)spacingX / texture->width,
			(float)spacingY / texture->height
		);
		font.uv_spacing.y *= -1.f;

		font.glyphsPerRow = (texture->width / (glyphWidth + spacingX));

		return font;
	}

	// Gets a glyph on the specified x and y coordinate
	void BuildGlyphAt(Glyph& outGlyph, int x, int y) const
	{
		outGlyph.pixel_size = pixel_size;
		outGlyph.uv_size = uv_size;
		outGlyph.uv_offset = Vec2(
			(uv_size.x + uv_spacing.x) * x,
			1.f + (uv_size.y + uv_spacing.y) * y
		);
	}

	// Calculate and find the glyph for at specific character
	void BuildGlyphForChar(Glyph& outGlyph, char c) const
	{
		int x = (int)c % glyphsPerRow;
		int y = (int)(c / glyphsPerRow);

		BuildGlyphAt(outGlyph, x, y);
	}

	// Build glyphs for a string
	uint32 BuildGlyphsForString(const char* str, Glyph* glyphArray, uint32 arrayNum) const
	{
		uint32 num = Math::Min<uint32>(strlen(str), arrayNum);
		for (uint32 i = 0; i < num; ++i)
		{
			BuildGlyphForChar(glyphArray[i], str[i]);
		}

		return num;
	}

	Texture* texture;

	Vec2 pixel_size;
	Vec2 pixel_spacing;
	Vec2 uv_size;
	Vec2 uv_spacing;
	int glyphsPerRow;
};
