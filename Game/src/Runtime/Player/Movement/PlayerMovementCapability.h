#pragma once
#include "Engine/Capability/Capability.h"
#include "Engine/Collision/Collision.h"

class TransformComponent;
class PlayerComponent;
class MovementComponent;

class PlayerMovementCapability : public Capability
{
public:
	void Setup() override;
	void Tick() override;

private:
	PlayerComponent* player = nullptr;
	TransformComponent* transform = nullptr;
	MovementComponent* movement = nullptr;

	float movement_speed = 5.f;
	float vertical_velocity = 0.f;
};
