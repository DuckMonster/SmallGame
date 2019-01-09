#pragma once
#include <vector>
#include "Core/Component/Component.h"

class Capability;

class CapabilityComponent : public Component
{
public:
	std::vector<Capability*> capability_list;

	template<typename TCapability>
	void AddCapability()
	{
		Capability* capability = new TCapability();
		capability->component = this;
		capability_list.push_back(capability);

		capability->Setup();
	}
};