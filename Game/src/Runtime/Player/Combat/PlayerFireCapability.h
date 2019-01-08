#pragma once
#include "Engine/Capability/Capability.h"

class TransformComponent;
class PlayerComponent;

class PlayerFireCapability : public Capability
{
public:
	void Setup();
	void Tick();

private:
	TransformComponent* transform;
	PlayerComponent* player;
};
