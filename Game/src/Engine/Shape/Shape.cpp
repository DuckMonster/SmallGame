
#include "Shape.h"
#include "Core/Scene/Scene.h"
#include "Core/Resource/MeshResource.h"
#include "Core/Resource/MaterialResource.h"
#include "Core/Resource/ResourceManager.h"
#include "Engine/Rendering/RenderableComponent.h"
#include "Engine/Transform/TransformComponent.h"

ShapePrefab ShapePrefab::Create(Scene* scene, const char* name, const char* mesh_path, const char* material_path)
{
	ShapePrefab result;
	result.entity = scene->CreateEntity(name);
	result.transform	= result.entity->AddComponent<TransformComponent>();
	result.renderable	= result.entity->AddComponent<RenderableComponent>();

	auto mesh		= gResourceManager->Load<MeshResource>(mesh_path);
	auto material	= gResourceManager->Load<MaterialResource>(material_path);

	result.renderable->mesh = &mesh->mesh;
	result.renderable->material = &material->material;

	return result;
}

ShapePrefab ShapePrefab::CreateCube(Scene* scene)
{
	return Create(scene, "Cube", "Mesh/cube.fbx", "Material/default.json");
}

ShapePrefab ShapePrefab::CreatePlane(Scene* scene)
{
	return Create(scene, "Plane", "Mesh/plane.fbx", "Material/default.json");
}

ShapePrefab ShapePrefab::CreateSphere(Scene* scene)
{
	return Create(scene, "Sphere", "Mesh/sphere.fbx", "Material/default.json");
}
