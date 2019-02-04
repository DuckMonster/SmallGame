#pragma once
#include "Core/System/System.h"
#include "ColliderComponent.h"

class CollisionSystem : public ComponentSystem<ColliderComponent>
{
public:
	void Setup() override;

protected:
	void ProcessEntity(Entity* entity, ColliderComponent* collider) override;
	void RunEnd() override;
};
