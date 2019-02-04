#pragma once
#include "Engine/Shape/Shape.h"
#include "Engine/Capability/Capability.h"
#include "Engine/Collision/Collision.h"

class PlayerComponent;
class TransformComponent;
class ColliderComponent;
class SphereCollider;
class BoxCollider;

class MouseAimCapability : public Capability
{
public:
	void Setup() override;
	void Tick() override;

private:
	PlayerComponent* player;
	TransformComponent* transform;
	ColliderComponent* collider;
};
