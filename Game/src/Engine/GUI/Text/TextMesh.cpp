
#include "TextMesh.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/TextureResource.h"
#include "Engine/GUI/Style/StyledStringParser.h"
#include "Engine/GUI/Style/StyleResource.h"

void TextMesh::Create()
{
	// Create the glyph mesh
	glyphMesh.vertexBuffer.Create(1);
	glyphMesh.vertexBuffer.BindBuffer(0, 0, 2, 8, 0);
	glyphMesh.vertexBuffer.BindBuffer(0, 1, 2, 8, 2);
	glyphMesh.vertexBuffer.BindBuffer(0, 2, 4, 8, 4);
	glyphMesh.drawMode = GL_TRIANGLES;
	glyphMesh.drawOffset = 0;
	glyphMesh.useElements = false;

	// Create the background mesh
	backgroundMesh.vertexBuffer.Create(1);
	backgroundMesh.vertexBuffer.BindBuffer(0, 0, 2, 6, 0);
	backgroundMesh.vertexBuffer.BindBuffer(0, 1, 4, 6, 2);
	backgroundMesh.drawMode = GL_TRIANGLES;
	backgroundMesh.drawOffset = 0;
	backgroundMesh.useElements = false;
}

void TextMesh::BuildString(const char* str)
{
	if (style == nullptr)
	{
		Error("Trying to build TextMesh with no given style");
		return;
	}

	const StylePalette& palette = style->palette;
	const Font& font = style->font->font;

	Glyph glyph;

	// Character-mesh offset
	int ox = 0;
	int oy = 1; // <- To leave some space for padding

	width = 0;
	height = 0;

	// Parse the string
	auto blocks = StyledStringParser::Parse(str, palette);
	glyphBuffer.Reserve(strlen(str));
	backBuffer.Reserve(blocks.Size());

	for (const StyledStringParser::Block& block : blocks)
	{
		int blockx = ox;
		int blocky = oy;

		bool newLine = false;

		const char* str = block.ptr;
		for (uint32 i = 0; i < block.length; ++i)
		{
			const char c = str[i];
			if (c == '\n')
			{
				// Defer newline onto later, because we wanna build the background first
				newLine = true;
				continue;
			}

			// Build glyph
			font.BuildGlyphForChar(glyph, c);
			GlyphQuad& quad = glyphBuffer.AddRef();
			quad.Build(glyph, ox, oy, block.style.foreground);

			// Add offset to next character
			ox += glyph.pixel_size.x;
		}

		// Create a background quad for this block
		BackgroundQuad& back = backBuffer.AddRef();
		back.Build(blockx, blocky, ox, blocky + font.pixel_size.y, 1, block.style.background);

		// Width is based on longest line, so update it
		if (ox > width)
			width = ox;
		height = blocky + font.pixel_size.y + 1;

		if (newLine)
		{
			// Now that we've built the background, we can apply the new-line
			oy += font.pixel_size.y + 2;
			ox = 0;
		}
	}

	// Upload glyph mesh data
	glyphMesh.vertexBuffer.BufferData(0, *glyphBuffer, sizeof(GlyphQuad) * glyphBuffer.Size());
	glyphMesh.drawCount = 6 * glyphBuffer.Size();

	// Upload background mesh data
	backgroundMesh.vertexBuffer.BufferData(0, *backBuffer, sizeof(BackgroundQuad) * backBuffer.Size());
	backgroundMesh.drawCount = 6 * backBuffer.Size();

	glyphBuffer.Clear();
	backBuffer.Clear();
}

void TextMesh::Draw(int x, int y, const Mat4& screen)
{
	if (style == nullptr)
		return;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind texture
	FontResource* font = style->font;
	Texture& texture = font->texture->texture;
	glBindTexture(GL_TEXTURE_2D, texture.handle);

	// Draw background
	Material& mBackground = font->backgroundMaterial->material;
	glUseProgram(mBackground.program);
	mBackground.Set("u_Offset", Vec2(x, y));
	mBackground.Set("u_Screen", screen);

	backgroundMesh.Draw();

	// Draw foreground
	Material& mForeground = font->foregroundMaterial->material;
	glUseProgram(mForeground.program);
	mBackground.Set("u_Offset", Vec2(x, y));
	mForeground.Set("u_Screen", screen);

	glyphMesh.Draw();

	glDisable(GL_BLEND);
}
