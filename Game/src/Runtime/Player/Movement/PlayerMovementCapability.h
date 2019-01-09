#pragma once
#include "Engine/Capability/Capability.h"

class TransformComponent;
class PlayerComponent;

class PlayerMovementCapability : public Capability
{
public:
	void Setup() override;
	void Tick() override;

private:
	TransformComponent* transform = nullptr;
	PlayerComponent* player = nullptr;

	float movement_speed = 5.f;
};
