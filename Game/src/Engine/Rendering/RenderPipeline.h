#pragma once
#include "RenderData.h"
#include "Core/Render/FrameBuffer.h"

class RenderPipeline
{
public:
	void Create();
	void Execute(RenderScene& scene);

private:
	void ExecuteGeometryPass(RenderScene& scene);
	void ExecuteShadowPass(RenderScene& scene);
	void ExecuteDebugPass(RenderScene& scene);

	FrameBuffer upscale_buffer;
	FrameBuffer shadow_buffer;
};
