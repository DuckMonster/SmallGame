#pragma once
#include "Plane.h"

struct Ray
{
	static Ray FromTo(const Vec3& from, const Vec3& to)
	{
		Ray ray;
		ray.origin = from;
		ray.direction = Vec::Normalize(to - from);

		return ray;
	}

	Vec3 origin;
	Vec3 direction;

	Vec3 GetPoint(float time)
	{
		return origin + direction * time;
	}

	Vec3 ProjectToPlane(const Plane& plane)
	{
		float h = Vec::Dot(origin, plane.normal);
		h = h - plane.distance;

		float d = Vec::Dot(plane.normal, -direction);
		if (d <= 0.f)
		{
			return Vec3::zero;
		}

		return GetPoint(h / d);
	}
};