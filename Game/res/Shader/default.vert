#version 330 core

in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_UV;

uniform mat4 u_Model;
uniform mat4 u_Camera;

out struct
{
	vec3 normal;
	vec2 uv;
	vec3 world;
} v2f;

void main()
{
	vec4 world = u_Model * vec4(a_Position, 1.0);
	gl_Position = u_Camera * world;
	v2f.normal = normalize((u_Model * vec4(a_Normal, 0.0)).xyz);
	v2f.uv = a_UV;
	v2f.world = world.xyz;
}