#pragma once
#include "Core/Component/Component.h"
#include "Engine/Capability/Capability.h"

class Scene;
class TransformComponent;
class CameraComponent;
class PlayerComponent;

// COMPONENT
class FollowCameraComponent : public Component
{
public:
	void SetTarget(Entity* entity);
	TransformComponent* target;
};

// CAPABILITY
class FollowCameraCapability : public Capability
{
public:
	void Setup() override;
	void Tick() override;

private:
	Vec3 offset = Vec3(0.f, 1.f, 1.f);
	float offsetDistance = 10.f;
	Vec3 root;
	float rotateAngle = 0.f;

	float zoom = 1.f;

	FollowCameraComponent* follow;
	CameraComponent* camera;
};

// PREFAB
struct FollowCamera
{
	static FollowCamera Create(Scene* scene, Entity* target);

	Entity* entity;
	FollowCameraComponent* component;
	CameraComponent* camera;
};
