#version 330 core

in vec3 a_Position;

uniform vec3 u_Origin;
uniform float u_Radius;
uniform vec4 u_Color;
uniform mat4 u_Camera;

out vec4 f_Color;

void main()
{
	vec3 world = a_Position;
	world = world * u_Radius + u_Origin;
	gl_Position = u_Camera * vec4(world, 1.0);
	f_Color = u_Color;
}