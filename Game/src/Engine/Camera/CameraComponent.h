#pragma once
#include "Core/Context/Context.h"
#include "Core/Component/Component.h"

class CameraComponent : public Component
{
public:
	Vec3 position;
	Vec3 forward = Vec3(1.f, 0.f, 0.f);
	Vec3 up = Vec3(0.f, 1.f, 0.f);

	void GetMatrix(Mat4& matrix);
	void GetMatrixInv(Mat4& matrix);
};