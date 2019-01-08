#pragma once
#include "Core/Component/Component.h"
class CameraComponent;

class RenderStaticComponent : public Component
{
public:
	CameraComponent* activeCamera;
};
