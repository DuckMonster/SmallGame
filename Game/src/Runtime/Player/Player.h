#pragma once
#include "Core/Component/Component.h"

class Scene;
class TransformComponent;
class RenderableComponent;
class ColliderComponent;
class CameraComponent;

// COMPONENT
class PlayerComponent : public Component
{
public:
	Vec3 aim_world_position;
	CameraComponent* follower_camera = nullptr;
	Entity* mesh_entity = nullptr;
};

// PREFAB
struct PlayerPrefab
{
	static PlayerPrefab Create(Scene* scene);

	Entity* entity;
	PlayerComponent* component;
	TransformComponent* transform;
	ColliderComponent* collider;

	Entity* mesh_entity;
	TransformComponent* mesh_transform;
	RenderableComponent* mesh_renderable;
};
