#pragma once
#include "Core/Component/Component.h"
#include "Core/Render/Material.h"
#include "Core/Render/VertexBuffer.h"
#include "RenderData.h"

class RenderableComponent : public Component
{
public:
	Mesh* mesh;
	Material* material;
	uint32 pass_flag = RP_Geometry | RP_Shadow;
};