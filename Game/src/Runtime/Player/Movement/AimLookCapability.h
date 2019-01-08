#pragma once
#include "Engine/Capability/Capability.h"

class PlayerComponent;
class TransformComponent;

class AimLookCapability : public Capability
{
public:
	void Setup() override;
	void Tick() override;

private:
	PlayerComponent* player;
	TransformComponent* transform;

	float angle = 0.f;
};

