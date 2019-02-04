#pragma once
#include "Core/Component/Component.h"

class DirectionalLightComponent : public Component
{
public:
	Vec3 position = Vec3::zero;
	Vec3 direction = Vec3::axis_x;
};