#pragma once
#include "Engine/Capability/Capability.h"

class PlayerComponent;
class TransformComponent;
class MovementComponent;

class AimLookCapability : public Capability
{
public:
	void Setup() override;
	void Tick() override;

private:
	PlayerComponent* player;
	TransformComponent* transform;
	MovementComponent* movement;

	float angle = 0.f;
};

