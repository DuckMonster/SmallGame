#include "RenderPipeline.h"
#include "Core/Context/Context.h"
#include "Core/Render/FullscreenQuad.h"
#include "Engine/Debug/DebugDrawManager.h"
#include "Engine/Debug/Debug.h"

void RenderPipeline::Create()
{
	// Create upscale buffer
	int width = gContext->width / 4;
	int height = gContext->height / 4;

	upscale_buffer.Create(width, height);
	upscale_buffer.AddTexture();
	upscale_buffer.AddDepthTexture();

	Assert(upscale_buffer.IsComplete());

	// Create shadow buffer
	shadow_buffer.Create(4096, 4096);
	shadow_buffer.AddDepthTexture();

	Assert(shadow_buffer.IsComplete());
}

void RenderPipeline::Execute(RenderScene& scene)
{
	shadow_buffer.Bind();
	ExecuteShadowPass(scene);
	FrameBuffer::Clear();

	upscale_buffer.Bind();
	ExecuteGeometryPass(scene);
	FrameBuffer::Clear();
	ExecuteDebugPass(scene);

	// Draw to screen, baby!
	FullscreenQuad::RenderTexture(upscale_buffer.textures[0]);
}

void RenderPipeline::ExecuteGeometryPass(RenderScene& scene)
{
	Texture& shadow_texture = shadow_buffer.textures[0];
	shadow_texture.Bind(1);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Mat4 light_proj = Mat4::Ortho(-20.f, 20.f, -20.f, 20.f, -40.f, 40.f);
	Mat4 light_view = Mat4::LookAtForward(scene.light.position, scene.light.direction, Vec3::up);
	Mat4 light_mat = light_proj * light_view;
	Mat4 light_mat_inv = Mat4::Inverse(light_mat);

	for (uint32 i = 0; i < scene.object_num; ++i)
	{
		RenderObject& object = scene.objects[i];
		glUseProgram(object.material.program);

		// Bind texture
		Texture& texture = object.material.texture;
		if (texture.IsValid())
			texture.Bind();

		object.material.Set("u_Sampler", 0);

		// Bind shadow depth texture
		object.material.Set("u_ShadowSampler", 1);

		object.material.Set("u_Model", object.matrix);
		object.material.Set("u_Camera", scene.camera.matrix);
		object.material.Set("u_Light.position", scene.light.position);
		object.material.Set("u_Light.direction", scene.light.direction);
		object.material.Set("u_Light.matrix", light_mat);
		object.material.Set("u_Light.matrix_inv", light_mat_inv);

		object.mesh.Draw();
	}
}

void RenderPipeline::ExecuteShadowPass(RenderScene& scene)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Mat4 light_proj = Mat4::Ortho(-20.f, 20.f, -20.f, 20.f, -40.f, 40.f);
	Mat4 light_view = Mat4::LookAtForward(scene.light.position, scene.light.direction, Vec3::up);
	Mat4 light_mat = light_proj * light_view;

	for (uint32 i = 0; i < scene.object_num; ++i)
	{
		RenderObject& object = scene.objects[i];
		glUseProgram(object.material.program);

		object.material.Set("u_Model", object.matrix);
		object.material.Set("u_Camera", light_mat);

		object.mesh.Draw();
	}
}

void RenderPipeline::ExecuteDebugPass(RenderScene& scene)
{
	glEnable(GL_DEPTH_TEST);
	gDebugDrawManager->DrawAndClear(scene.camera.matrix);
}
