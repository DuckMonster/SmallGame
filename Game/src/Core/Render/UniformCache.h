#pragma once
#include "Core/Standard/Map.h"
class UniformCache
{
	struct UniformMap
	{
		GLuint Get(const char* name);

		GLuint program;
		Map<String, GLuint> map;
	};

public:
	static GLuint Get(GLuint program, const char* name);
	static void InvalidateCacheFor(GLuint program);

private:

	static Map<GLuint, UniformMap> map;
	static UniformMap* hot_map;
};
