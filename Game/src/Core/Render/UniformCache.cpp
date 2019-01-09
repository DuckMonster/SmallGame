
#include "UniformCache.h"

Map<GLuint, UniformCache::UniformMap> UniformCache::map;
UniformCache::UniformMap* UniformCache::hot_map = nullptr;

GLuint UniformCache::UniformMap::Get(const char* name)
{
	GLuint* result = map.Find(name);
	if (result == nullptr)
	{
		glUseProgram(program);
		result = &map.Add(name, glGetUniformLocation(program, name));
	}

	return *result;
}

GLuint UniformCache::Get(GLuint program, const char* name)
{
	// Is this program hot right now? (Skip one map-lookup)
	if (hot_map != nullptr && hot_map->program == program)
	{
		return hot_map->Get(name);
	}

	// Otherwise, just look it up
	UniformMap& program_map = map[program];
	program_map.program = program;
	GLuint result = program_map.Get(name);

	// Make this program hot
	hot_map = &program_map;

	return result;
}

void UniformCache::InvalidateCacheFor(GLuint program)
{
	// If this program was hot, make sure it isnt
	if (hot_map != nullptr)
	{
		if (hot_map->program == program)
			hot_map = nullptr;
	}

	map.Remove(program);
}
