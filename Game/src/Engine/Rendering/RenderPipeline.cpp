
#include "RenderPipeline.h"
#include "Core/Context/Context.h"
#include "Core/Render/FullscreenQuad.h"
#include "Engine/Debug/DebugDrawManager.h"

namespace
{
	void ExecuteGeometryPass(RenderScene& scene)
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		for (uint32 i = 0; i < scene.numObjects; ++i)
		{
			RenderObject& object = scene.objects[i];
			glUseProgram(object.material.program);

			if (object.material.texture.IsValid())
				glBindTexture(GL_TEXTURE_2D, object.material.texture.handle);

			object.material.Set("u_Model", object.matrix);
			object.material.Set("u_Camera", scene.camera.matrix);

			object.mesh.Draw();
		}
	}

	void ExecuteDebugPass(RenderScene& scene)
	{
		glEnable(GL_DEPTH_TEST);
		gDebugDrawManager->DrawAndClear(scene.camera.matrix);
	}
}

void RenderPipeline::Create()
{
	int width = gContext->width / 4;
	int height = gContext->height / 4;

	upscaleBuffer.Create(width, height);
	upscaleBuffer.AddTexture();
	upscaleBuffer.AddDepthTexture();

	Assert(upscaleBuffer.IsValid() && upscaleBuffer.IsComplete());
}

void RenderPipeline::Execute(RenderScene& scene)
{
	upscaleBuffer.Bind();
	ExecuteGeometryPass(scene);
	ExecuteDebugPass(scene);
	FrameBuffer::Clear();

	// Draw to screen, baby!
	FullscreenQuad::RenderTexture(upscaleBuffer.textures[0]);
}
