#pragma once
#include "Core/Component/Component.h"

class CameraComponent;
class DirectionalLightComponent;

class RenderStaticComponent : public Component
{
public:
	CameraComponent* active_camera = nullptr;
	DirectionalLightComponent* light = nullptr;
};
