#pragma once
#include <GL/glew.h>
#include "Texture.h"
#include "Core/Standard/Map.h"
#include "UniformCache.h"

struct Material
{
	static bool LoadSource(Material& material, const char* vert, const char* frag);

	GLuint program;
	Texture texture;

	void Set(const char* anme, float value);
	void Set(const char* name, const Vec2& value);
	void Set(const char* name, const Vec3& value);
	void Set(const char* name, const Vec4& value);
	void Set(const char* name, const Mat4& value);
	void Set(const char* name, const Color& value);
};