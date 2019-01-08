#pragma once

class Scene;
class Entity;
class TransformComponent;
class RenderableComponent;

struct ShapePrefab
{
	static ShapePrefab Create(Scene* scene, const char* name, const char* meshPath, const char* materialPath);
	static ShapePrefab CreateCube(Scene* scene);
	static ShapePrefab CreatePlane(Scene* scene);
	static ShapePrefab CreateSphere(Scene* scene);

	Entity* entity;
	TransformComponent* transform;
	RenderableComponent* renderable;
};