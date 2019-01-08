
#include "UniformCache.h"

Map<GLuint, UniformCache::UniformMap> UniformCache::map;
UniformCache::UniformMap* UniformCache::hotMap = nullptr;

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
	if (hotMap != nullptr && hotMap->program == program)
	{
		return hotMap->Get(name);
	}

	// Otherwise, just look it up
	UniformMap& programMap = map[program];
	programMap.program = program;
	GLuint result = programMap.Get(name);

	// Make this program hot
	hotMap = &programMap;

	return result;
}

void UniformCache::InvalidateCacheFor(GLuint program)
{
	// If this program was hot, make sure it isnt
	if (hotMap != nullptr)
	{
		if (hotMap->program == program)
			hotMap = nullptr;
	}

	map.Remove(program);
}
