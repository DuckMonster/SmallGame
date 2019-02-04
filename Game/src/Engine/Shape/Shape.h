#pragma once

class Scene;
class Entity;
class TransformComponent;
class RenderableComponent;
class ColliderComponent;

struct ShapePrefab
{
	static ShapePrefab Create(Scene* scene, const Transform& transform, const char* name, const char* mesh_path, const char* material_path);
	static ShapePrefab CreateCube(Scene* scene, bool add_collider, const Transform& transform = Transform::identity);
	static ShapePrefab CreatePlane(Scene* scene, bool add_collider, const Transform& transform = Transform::identity);
	static ShapePrefab CreateSphere(Scene* scene, bool add_collider, const Transform& transform = Transform::identity);

	Entity* entity;
	TransformComponent* transform;
	RenderableComponent* renderable;
	ColliderComponent* collider = nullptr;
};