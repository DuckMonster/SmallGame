#version 330 core
uniform sampler2D u_Sampler;
uniform sampler2D u_ShadowSampler;

const float SHADOW_BIAS = {shadow_bias};
const vec3 COLOR = {color};
const vec3 COLOR_SHADOW = {color_shadow};
const float SHADOW_ALPHA = {shadow_alpha};
const float AMBIENT = {ambient};

in struct
{
	vec3 normal;
	vec2 uv;
	vec3 world;
} v2f;

struct Light
{
	vec3 position;
	vec3 direction;
	mat4 matrix;
	mat4 matrix_inv;
};

uniform Light u_Light;

out vec4 o_Color;

void main()
{
	/****** Diffuse ******/
	float diffuse = dot(u_Light.direction, -v2f.normal);
	diffuse *= step(0.0, diffuse);

	/****** Shadow ******/
	// Project world position onto the light NDC
	vec4 light_ndc = u_Light.matrix * vec4(v2f.world, 1.0);

	// Perspective divide
	light_ndc /= light_ndc.w;

	// [-1, 1] => [0, 1]
	light_ndc = light_ndc * 0.5 + 0.5;

	// Compare depth value to texture
	float light_depth = texture(u_ShadowSampler, light_ndc.xy).x;
	float shadow = (light_depth > (light_ndc.z - SHADOW_BIAS)) ? 1.0 : SHADOW_ALPHA;

	// Just cut off shadow if the diffuse is nearly zero
	shadow = diffuse < 0.1 ? 0.0 : shadow;
	diffuse *= mix(SHADOW_ALPHA, 1.0, shadow);

	vec3 color = mix(COLOR_SHADOW, COLOR, shadow);

	o_Color = vec4(color * (diffuse + AMBIENT), 1.0);
	if (o_Color.a < 0.2)
		discard;
}