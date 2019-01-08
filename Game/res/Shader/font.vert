#version 330 core
in vec2 a_Position;
in vec2 a_UV;
in vec4 a_Color;

uniform mat4 u_Screen;
uniform vec2 u_Offset;

out vec2 f_UV;
out vec4 f_Color;

void main()
{
	gl_Position = u_Screen * vec4(a_Position + u_Offset, 0.0, 1.0);
	f_UV = a_UV;
	f_Color = a_Color;
}