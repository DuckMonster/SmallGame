#pragma once
#include "Font.h"
#include "Core/Render/Mesh.h"
#include "Core/Render/Material.h"

class StyleResource;

// Data for describing the position and uv of a single glyph in a string
struct GlyphQuad
{
	// On vertex in the mesh
	struct Vertex
	{
		Vec2 position;
		Vec2 uv;
		Color color;
	};

	// Build the mesh based on a font-glyph and a (x, y) coordinate
	void Build(const Glyph& glyph, int x, int y, const Color& color)
	{
		struct
		{
			// Helps build a single vertex
			static void BuildVertex(Vertex& v, const Glyph& glyph, const Vec2& offset, const Vec2& vPos, const Color& color)
			{
				// vPos describes its position on the quad (from [0, 1] in x and y)
				v.position	= offset + glyph.pixel_size * vPos;
				v.uv		= glyph.uv_offset + glyph.uv_size * vPos;
				v.color		= color;
			}
		} helper;

		Vec2 offset(x, y);
		Vec2 v(0.f, 1.f);
		Vec2 h(1.f, 0.f);

		/* FIRST TRIANGLE */
		// Top-left
		helper.BuildVertex(vertices[0], glyph, offset, Vec2(0.f), color);

		// Bottom-left
		helper.BuildVertex(vertices[1], glyph, offset, v, color);

		// Top-right
		helper.BuildVertex(vertices[2], glyph, offset, h, color);

		/* SECOND TRIANGLE */
		// Bottom-left
		helper.BuildVertex(vertices[3], glyph, offset, v, color);
		
		// Top-right
		helper.BuildVertex(vertices[4], glyph, offset, h, color);

		// Bottom-right
		helper.BuildVertex(vertices[5], glyph, offset, v + h, color);
	}

	Vertex vertices[6];
};

// Describes a background-quad thats drawn behind the glyphs
struct BackgroundQuad
{
	struct Vertex
	{
		Vec2 position;
		Color color;
	};

	void Build(int x, int y, int endx, int endy, int padding, const Color& color)
	{
		struct
		{
			// Helps build a single vertex
			static void BuildVertex(Vertex& v, const Vec2& offset, const Vec2& size, const Vec2& vPos, const Color& color)
			{
				// vPos describes its position on the quad (from [0, 1] in x and y)
				v.position	= offset + size * vPos;
				v.color = color;
			}
		} helper;

		Vec2 offset(x, y - padding);
		Vec2 size((endx - x), (endy - y) + padding * 2);
		Vec2 v(0.f, 1.f);
		Vec2 h(1.f, 0.f);

		/* FIRST TRIANGLE */
		// Top-left
		helper.BuildVertex(vertices[0], offset, size, Vec2(0.f), color);

		// Bottom-left
		helper.BuildVertex(vertices[1], offset, size, v, color);

		// Top-right
		helper.BuildVertex(vertices[2], offset, size, h, color);

		/* SECOND TRIANGLE */
		// Bottom-left
		helper.BuildVertex(vertices[3], offset, size, v, color);
		
		// Top-right
		helper.BuildVertex(vertices[4], offset, size, h, color);

		// Bottom-right
		helper.BuildVertex(vertices[5], offset, size, v + h, color);
	}

	Vertex vertices[6];
};

class TextMesh
{
public:
	void Create();
	void BuildString(const char* str);
	void Draw(int x, int y, const Mat4& screen);

	int width = 0;
	int height = 0;

	StyleResource* style = nullptr;

	Mesh glyphMesh;
	Mesh backgroundMesh;

	Array<GlyphQuad> glyphBuffer;
	Array<BackgroundQuad> backBuffer;
};