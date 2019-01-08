#pragma once
#include "Core/Entity/Entity.h"
#include "Engine/Capability/CapabilityComponent.h"

template<typename TCapability>
void AddCapability(Entity* entity)
{
	entity->GetOrAddComponent<CapabilityComponent>()->AddCapability<TCapability>();
}
