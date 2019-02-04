#pragma once
#include "Core/System/System.h"
#include "ColliderComponent.h"

class CollisionDebugSystem : public ComponentSystem<ColliderComponent>
{
protected:
	void RunBegin() override;
	void ProcessEntity(Entity* entity, ColliderComponent* collider) override;

private:
	bool debug_enabled = false;
};