#pragma once
#include "Core/Math/Ray.h"

class Camera
{
public:
	static Ray Deproject(const Vec2& mouse, const Mat4& camera_inv);
};