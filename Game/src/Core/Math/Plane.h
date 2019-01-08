#pragma once
struct Plane
{
	static Plane FromPointNormal(Vec3 point, Vec3 normal)
	{
		Plane plane;
		plane.normal = normal;
		plane.distance = Vec::Dot(point, normal);

		return plane;
	}

	Vec3 normal;
	float distance;
};
