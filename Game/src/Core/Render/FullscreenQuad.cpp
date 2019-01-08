
#include "FullscreenQuad.h"
#include "Material.h"
#include "Mesh.h"

namespace
{
	const char* QUAD_VERT = ""
		"#version 330 core\n"
		"const vec2 verts[4] = vec2[4]("
		"	vec2(-1.0, -1.0),"
		"	vec2(1.0, -1.0),"
		"	vec2(1.0, 1.0),"
		"	vec2(-1.0, 1.0)"
		");"
		"const vec2 uvs[4] = vec2[4]("
		"	vec2(0.0, 0.0),"
		"	vec2(1.0, 0.0),"
		"	vec2(1.0, 1.0),"
		"	vec2(0.0, 1.0)"
		");"

		"out vec2 f_UV;"

		"void main() {"
		"	gl_Position = vec4(verts[gl_VertexID], -1.0, 1.0);"
		"	f_UV = uvs[gl_VertexID];"
		"}";

	const char* QUAD_FRAG = ""
		"#version 330 core\n"
		"uniform sampler2D u_Sampler;"
		"in vec2 f_UV;"
		"out vec4 o_Color;"

		"void main() {"
		"	o_Color = texture(u_Sampler, f_UV);"
		"}";
}

void FullscreenQuad::RenderTexture(const Texture& texture)
{
	static GLuint vao;
	static Material quadMaterial;
	static bool materialLoaded = false;

	if (!materialLoaded)
	{
		// We need a dummy VAO
		glGenVertexArrays(1, &vao);
		Material::LoadSource(quadMaterial, QUAD_VERT, QUAD_FRAG);
		materialLoaded = true;
	}

	glBindVertexArray(vao);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(quadMaterial.program);
	glBindTexture(GL_TEXTURE_2D, texture.handle);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
}
