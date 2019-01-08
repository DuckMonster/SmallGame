#version 330 core
uniform sampler2D u_Sampler;

in struct
{
	vec3 normal;
	vec2 uv;
} v2f;

out vec4 o_Color;

void main()
{
	o_Color = vec4(0.4f, 0.4f, 0.1f, 1.0);
}