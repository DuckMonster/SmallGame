#pragma once
#include "Core/Entity/Entity.h"

class CapabilityComponent;

class Capability
{
	friend class CapabilityComponent;

public:
	virtual ~Capability() { }
	virtual void Setup() {}
	virtual void Tick() {}

	Entity* GetOwner();

private:
	CapabilityComponent* component;
};
