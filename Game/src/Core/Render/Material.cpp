
#include "Material.h"

GLuint CreateAndCompileShader(GLenum type, const char* src)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	// Check for compile errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Uh oh
		char* buffer = talloc(1024);
		glGetShaderInfoLog(shader, 1024, nullptr, buffer);
		printf("====== SHADER COMPILE FAILED ======\n%s\n===================================\n", buffer);
	}

	return shader;
}

bool Material::LoadSource(Material& material, const char* vert, const char* frag)
{
	GLuint program = glCreateProgram();
	material.program = program;

	GLuint vertShader = CreateAndCompileShader(GL_VERTEX_SHADER, vert);
	GLuint fragShader = CreateAndCompileShader(GL_FRAGMENT_SHADER, frag);
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Uh oh
		char* buffer = talloc(1024);
		glGetProgramInfoLog(program, 1024, nullptr, buffer);
		printf("====== PROGRAM LINK FAILED ======\n%s\n=================================\n", buffer);

		return false;
	}

	return true;
}

void Material::Set(const char* name, float value)
{
	GLuint uniform = UniformCache::Get(program, name);
	if (uniform == -1)
		return;

	glUniform1f(uniform, value);
}

#define DEF_UNIFORM(type, func)\
void Material::Set(const char* name, const type& value)\
{\
	GLuint uniform = UniformCache::Get(program, name);\
	if (uniform == -1)\
		return;\
\
	func(uniform, 1, (float*)&value);\
}

DEF_UNIFORM(Vec2, glUniform2fv);
DEF_UNIFORM(Vec3, glUniform3fv);
DEF_UNIFORM(Vec4, glUniform4fv);
DEF_UNIFORM(Color, glUniform4fv);

void Material::Set(const char* name, const Mat4& value)
{
	GLuint uniform = UniformCache::Get(program, name);
	if (uniform == -1)
		return;

	glUniformMatrix4fv(uniform, 1, false, (float*)&value);
}
