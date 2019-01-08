#version 330 core

in vec3 a_Position;

uniform vec4 u_Color;
uniform mat4 u_Model;
uniform mat4 u_Camera;

out vec4 f_Color;

void main()
{
	gl_Position = u_Camera * u_Model * vec4(a_Position, 1.0);
	f_Color = u_Color;
}