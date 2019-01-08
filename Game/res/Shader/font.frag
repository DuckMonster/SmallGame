#version 330 core
uniform sampler2D u_Sampler;

in vec2 f_UV;
in vec4 f_Color;
out vec4 o_Color;

void main()
{
	o_Color = texture(u_Sampler, f_UV);
	if (o_Color.r < 0.5)
		discard;

	o_Color *= f_Color;
}