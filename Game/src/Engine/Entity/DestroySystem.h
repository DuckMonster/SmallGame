#pragma once
#include "Core/System/System.h"
#include "DestroyComponent.h"

class DestroySystem : public ComponentSystem<DestroyComponent>
{
public:
	void ProcessEntity(Entity* entity, DestroyComponent* destroy)
	{
		delete entity;
	}
};
