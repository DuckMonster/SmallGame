#pragma once
#include "Core/System/System.h"
#include "ColliderComponent.h"

class CollisionSystem : public System
{
public:
	void Setup() override;

protected:
	void Run() override;
};
