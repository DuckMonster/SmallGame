#pragma once
#include "RenderData.h"
#include "Core/Render/FrameBuffer.h"

class RenderPipeline
{
public:
	void Create();
	void Execute(RenderScene& scene);

	FrameBuffer upscaleBuffer;
};
