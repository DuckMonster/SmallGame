
#include "Camera.h"
#include "Core/Context/Context.h"

Ray Camera::Deproject(const Vec2& mouse, const Mat4& cameraInv)
{
	Vec2 viewportSize(
		gContext->width,
		gContext->height
	);

	// NDC coordinates
	Vec2 normalized = (mouse / viewportSize) * 2.f - 1.f;
	normalized.y *= -1.f; // invert y

	// Project
	Vec4 near = cameraInv * Vec4(normalized, -1.f, 1.f);
	Vec4 far = cameraInv * Vec4(normalized, 1.f, 1.f);

	// Perspective divide
	far /= far.w;
	near /= near.w;

	// Create ray
	Ray ray;
	ray.origin = Vec3(near);
	ray.direction = Vec::Normalize(Vec3(far) - Vec3(near));

	return ray;
} 