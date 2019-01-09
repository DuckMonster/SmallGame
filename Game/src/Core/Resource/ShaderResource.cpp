
#include "ShaderResource.h"
#include "Core/OS/File.h"

namespace
{
	GLenum GetShaderTypeFromFileName(const char* path)
	{
		if (strstr(path, ".vert") != nullptr)
			return GL_VERTEX_SHADER;
		if (strstr(path, ".frag") != nullptr)
			return GL_FRAGMENT_SHADER;

		Error("'%s' is not a valid shader file!", path);
		return GL_INVALID_ENUM;
	}
}

bool ShaderResource::LoadInternal(const char* path)
{
	// Infer shader type from file
	GLenum type = GetShaderTypeFromFileName(path);
	if (type == GL_INVALID_ENUM)
		return false;

	// Read file
	char* source;
	File::ReadAllTemporary(path, source);

	// Create and compile shader
	handle = glCreateShader(type);
	glShaderSource(handle, 1, &source, nullptr);
	glCompileShader(handle);

	// Check for compile errors
	GLint success;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Uh oh
		char* buffer = talloc(1024);
		glGetShaderInfoLog(handle, 1024, nullptr, buffer);
		Debug_Log("COMPILE ERROR IN SHADER (%s)\n%s", path, buffer);
	}
	else
	{
		is_valid = true;
	}

	return handle;
}

void ShaderResource::UnloadInternal()
{
	glDeleteShader(handle);
	handle = GL_INVALID_INDEX;
	is_valid = false;
}

