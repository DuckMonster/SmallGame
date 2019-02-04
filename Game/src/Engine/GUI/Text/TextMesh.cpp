
#include "TextMesh.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/TextureResource.h"
#include "Engine/GUI/Style/StyledStringParser.h"
#include "Engine/GUI/Style/StyleResource.h"

void TextMesh::Create()
{
	// Create the glyph mesh
	mesh_glyph.vertex_buffer.Create(1);
	mesh_glyph.vertex_buffer.BindBuffer(0, 0, 2, 8, 0);
	mesh_glyph.vertex_buffer.BindBuffer(0, 1, 2, 8, 2);
	mesh_glyph.vertex_buffer.BindBuffer(0, 2, 4, 8, 4);
	mesh_glyph.draw_mode = GL_TRIANGLES;
	mesh_glyph.draw_offset = 0;
	mesh_glyph.use_elements = false;

	// Create the background mesh
	mesh_background.vertex_buffer.Create(1);
	mesh_background.vertex_buffer.BindBuffer(0, 0, 2, 6, 0);
	mesh_background.vertex_buffer.BindBuffer(0, 1, 4, 6, 2);
	mesh_background.draw_mode = GL_TRIANGLES;
	mesh_background.draw_offset = 0;
	mesh_background.use_elements = false;
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
	glyph_buffer.Reserve(strlen(str));
	back_buffer.Reserve(blocks.Size());

	for (const StyledStringParser::Block& block : blocks)
	{
		int blockx = ox;
		int blocky = oy;

		bool new_line = false;

		const char* str = block.ptr;
		for (uint32 i = 0; i < block.length; ++i)
		{
			const char c = str[i];
			if (c == '\n')
			{
				// Defer newline onto later, because we wanna build the background first
				new_line = true;
				continue;
			}

			// Build glyph
			font.BuildGlyphForChar(glyph, c);
			GlyphQuad& quad = glyph_buffer.AddRef();
			quad.Build(glyph, ox, oy, block.style.foreground);

			// Add offset to next character
			ox += glyph.pixel_size.x;
		}

		// Create a background quad for this block
		BackgroundQuad& back = back_buffer.AddRef();
		back.Build(blockx, blocky, ox, blocky + font.pixel_size.y, 1, block.style.background);

		// Width is based on longest line, so update it
		if (ox > width)
			width = ox;
		height = blocky + font.pixel_size.y + 1;

		if (new_line)
		{
			// Now that we've built the background, we can apply the new-line
			oy += font.pixel_size.y + 2;
			ox = 0;
		}
	}

	// Upload glyph mesh data
	mesh_glyph.vertex_buffer.BufferData(0, *glyph_buffer, sizeof(GlyphQuad) * glyph_buffer.Size());
	mesh_glyph.draw_count = 6 * glyph_buffer.Size();

	// Upload background mesh data
	mesh_background.vertex_buffer.BufferData(0, *back_buffer, sizeof(BackgroundQuad) * back_buffer.Size());
	mesh_background.draw_count = 6 * back_buffer.Size();

	glyph_buffer.Clear();
	back_buffer.Clear();
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
	texture.Bind();

	// Draw background
	Material& mat_background = font->background_material->material;
	glUseProgram(mat_background.program);
	mat_background.Set("u_Offset", Vec2(x, y));
	mat_background.Set("u_Screen", screen);

	mesh_background.Draw();

	// Draw foreground
	Material& mat_foreground = font->foreground_material->material;
	glUseProgram(mat_foreground.program);
	mat_background.Set("u_Offset", Vec2(x, y));
	mat_foreground.Set("u_Screen", screen);

	mesh_glyph.Draw();

	glDisable(GL_BLEND);
}
