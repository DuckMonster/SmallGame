#pragma once
#include <GL/glew.h>
#include <stdio.h>

inline void __stdcall OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLuint severity,
	GLsizei length, const GLchar* message, const void* userParam)
{
	printf("OPENGL ERROR (source: %X, type: %X, id: %X)\n%s\n", source, type, id, message);
}

