
#include "Core/Class/TypeHelpers.h"
#include "RenderableComponent.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/MeshResource.h"
#include "Core/Resource/MaterialResource.h"

REGISTER_TYPE(RenderableComponent);

RenderableComponent::Renderable* RenderableComponent::AddRenderable(Mesh* mesh, Material* material, const Mat4& matrix/* = Mat4::identity*/)
{
	Renderable& renderable = renderables.AddRef();
	renderable.mesh = mesh;
	renderable.material = material;
	renderable.matrix = matrix;

	return &renderable;
}
RenderableComponent::Renderable* RenderableComponent::AddRenderableLoad(const char* mesh_path, const char* material_path, const Mat4& matrix/* = Mat4::identity*/)
{
	Renderable& renderable = renderables.AddRef();
	renderable.mesh = &gResourceManager->Load<MeshResource>(mesh_path)->mesh;
	renderable.material = &gResourceManager->Load<MaterialResource>(material_path)->material;
	renderable.matrix = matrix;

	return &renderable;
}