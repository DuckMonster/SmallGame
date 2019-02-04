#pragma once
#include "Core/Component/Component.h"
#include "Core/Render/Material.h"
#include "Core/Render/VertexBuffer.h"
#include "RenderData.h"

class RenderableComponent : public Component
{
public:
	struct Renderable
	{
		Mesh* mesh = nullptr;
		Material* material = nullptr;
		Mat4 matrix = Mat4(1.f);
		uint32 pass_flag = RP_Geometry | RP_Shadow;
	};

	Array<Renderable> renderables;
	Renderable* AddRenderable(Mesh* mesh, Material* material, const Mat4& matrix = Mat4::identity);
	Renderable* AddRenderableLoad(const char* mesh_path, const char* material_path, const Mat4& matrix = Mat4::identity);
};