
#include "Capability.h"
#include "CapabilityComponent.h"

Entity* Capability::GetOwner()
{
	return component->GetOwner();
}
