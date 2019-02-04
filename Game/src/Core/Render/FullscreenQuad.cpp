
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
		"uniform mat4 u_Screen;"

		"out vec2 f_UV;"

		"void main() {"
		"	gl_Position = u_Screen * vec4(verts[gl_VertexID], -1.0, 1.0);"
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

void FullscreenQuad::RenderTexture(const Texture& texture, const Vec2& position, const Vec2& size)
{
	static GLuint vao;
	static Material quad_material;
	static bool material_loaded = false;

	if (!material_loaded)
	{
		// We need a dummy VAO
		glGenVertexArrays(1, &vao);
		Material::LoadSource(quad_material, QUAD_VERT, QUAD_FRAG);
		material_loaded = true;
	}

	Mat4 screen(1.f);
	screen.SetDiagonal(Vec4(size * 0.5f, 0.f, 1.f));
	screen[3] = Vec4(position, 0.f, 1.f);

	glBindVertexArray(vao);
	glDisable(GL_DEPTH_TEST);

	texture.Bind();
	glUseProgram(quad_material.program);
	quad_material.Set("u_Screen", screen);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
}
