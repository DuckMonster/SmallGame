
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
	// Read file
	char* source_c;
	File::ReadAllTemporary(path, source_c);

	source = source_c;
	return true;
}

void ShaderResource::UnloadInternal()
{
	source = "";
}

GLuint ShaderResource::Compile(const Map<String, String>& parameters)
{
	const char* path = *GetPath();
	TString src = *source;

	// Infer shader type
	GLenum type = GetShaderTypeFromFileName(path);
	if (type == GL_INVALID_ENUM)
	{
		Error("Shader '%s' is not a valid shader type", path);
		return -1;
	}

	// Replace parameters in source
	for(const KeyValuePair<String, String>& parameter : parameters)
	{
		TString param_name = TString::Printf("{%s}", *parameter.key);
		src = src.Replace(*param_name, *parameter.value);
	}

	const char* src_ptr = *src;

	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, &src_ptr, nullptr);
	glCompileShader(handle);

	// Check for errors
	GLint success;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Uh oh
		char* buffer = talloc(1024);
		glGetShaderInfoLog(handle, 1024, nullptr, buffer);
		Error("Compile error in shader '%s'\n'%s", path, buffer);
	}

	return handle;
}