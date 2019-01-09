#pragma once
#include "Core/System/System.h"
#include "CapabilityComponent.h"

class CapabilitySystem : public ComponentSystem<CapabilityComponent>
{
public:
	void ProcessEntity(Entity* entity, CapabilityComponent* capability_comp);
};
