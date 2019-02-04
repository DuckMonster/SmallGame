#pragma once
#include "Engine/Capability/Capability.h"
#include "Engine/Collision/Collision.h"

class Scene;
class TransformComponent;
class RenderableComponent;
class ColliderComponent;
class MovementComponent;

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
	void HandleOverlap(const OverlapResult& result);

	float time = 3.f;
	ProjectileComponent* projectile;
	MovementComponent* movement;

	Vec3 velocity;
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
