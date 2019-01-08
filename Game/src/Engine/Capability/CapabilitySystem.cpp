
#include "CapabilitySystem.h"
#include "Capability.h"

void CapabilitySystem::ProcessEntity(Entity* entity, CapabilityComponent* capabilityComponent)
{
	for (Capability* capability : capabilityComponent->capabilityList)
	{
		capability->Tick();
	}
}

