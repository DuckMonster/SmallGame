#pragma once
#include "Engine/Capability/Capability.h"

class Scene;
class TransformComponent;
class RenderableComponent;
class ColliderComponent;

// COMPONENT
class ProjectileComponent : public Component
{
public:
	Vec3 direction;
};

// CAPABILITY
class ProjectileCapability : public Capability
{
public:
	void Setup() override;
	void Tick() override;

private:
	void HandleOverlap(Entity* other);

	float time = 3.f;
	ProjectileComponent* projectile;
	TransformComponent* transform;
};

// PREFAB
class ProjectilePrefab
{
public:
	static ProjectilePrefab Create(Scene* scene, const Vec3& origin, const Vec3& direction);

	Entity* entity;
	TransformComponent* transform;
	ProjectileComponent* projectile;
	RenderableComponent* renderable;
	ColliderComponent* collider;
};
