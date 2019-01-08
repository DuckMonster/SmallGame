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
	//o_Color = texture(u_Sampler, v2f.uv);
	o_Color = vec4(v2f.normal, 1.0);
	if (o_Color.a < 0.2)
		discard;
}