#pragma once
#include "Core/Component/Component.h"

class Scene;
class TransformComponent;
class RenderableComponent;
class ColliderComponent;
class CameraComponent;
class MovementComponent;

// COMPONENT
class PlayerComponent : public Component
{
public:
	Vec3 aim_world_position;
	CameraComponent* follower_camera = nullptr;
};

// PREFAB
struct PlayerPrefab
{
	static PlayerPrefab Create(Scene* scene);

	Entity* entity;
	PlayerComponent* component;
	TransformComponent* transform;
	RenderableComponent* renderable;
	ColliderComponent* collider;
	MovementComponent* movement;
};
