#pragma once
#include "Core/System/System.h"
#include "MovementComponent.h"
#include "Engine/Transform/TransformComponent.h"

class MovementSystem : public ComponentSystem<TransformComponent, MovementComponent>
{
protected:
	void ProcessEntity(Entity* entity, TransformComponent* transform, MovementComponent* movement) override;
};