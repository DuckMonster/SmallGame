
#include "CapabilitySystem.h"
#include "Capability.h"

void CapabilitySystem::ProcessEntity(Entity* entity, CapabilityComponent* capability_comp)
{
	for (Capability* capability : capability_comp->capability_list)
	{
		capability->Tick();
	}
}

