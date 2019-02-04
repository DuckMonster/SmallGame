
#include "Shape.h"
#include "Core/Scene/Scene.h"
#include "Core/Resource/MeshResource.h"
#include "Core/Resource/MaterialResource.h"
#include "Core/Resource/ResourceManager.h"
#include "Engine/Rendering/RenderableComponent.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Collision/ColliderComponent.h"

ShapePrefab ShapePrefab::Create(Scene* scene, const Transform& transform, const char* name, const char* mesh_path, const char* material_path)
{
	ShapePrefab result;
	result.entity = scene->CreateEntity(name);
	result.transform	= result.entity->AddComponent<TransformComponent>();
	result.transform->transform = transform;

	result.renderable	= result.entity->AddComponent<RenderableComponent>();
	result.renderable->AddRenderableLoad(mesh_path, material_path);

	return result;
}

ShapePrefab ShapePrefab::CreateCube(Scene* scene, bool add_collider, const Transform& transform)
{
	ShapePrefab prefab = Create(scene, transform, "Cube", "Mesh/cube.fbx", "Material/default.json");
	if (add_collider)
	{
		prefab.collider = prefab.entity->AddComponent<ColliderComponent>();
		prefab.collider->object->AddBox(Vec3::zero, Vec3::one, Quat::identity);
	}

	return prefab;
}

ShapePrefab ShapePrefab::CreatePlane(Scene* scene, bool add_collider, const Transform& transform)
{
	ShapePrefab prefab = Create(scene, transform, "Plane", "Mesh/plane.fbx", "Material/default.json");
	if (add_collider)
	{
		prefab.collider = prefab.entity->AddComponent<ColliderComponent>();
		prefab.collider->object->AddBox(Vec3(0.f), Vec3(1.f, 0.f, 1.f), Quat::identity);
	}

	return prefab;
}

ShapePrefab ShapePrefab::CreateSphere(Scene* scene, bool add_collider, const Transform& transform)
{
	ShapePrefab prefab = Create(scene, transform, "Sphere", "Mesh/sphere.fbx", "Material/default.json");
	if (add_collider)
	{
		prefab.collider = prefab.entity->AddComponent<ColliderComponent>();
		prefab.collider->object->AddSphere(Vec3::zero, 1.f);
	}

	return prefab;
}
