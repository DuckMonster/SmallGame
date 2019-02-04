#pragma once

struct Transform
{
	const static Transform identity;

	Transform() {}
	Transform(Vec3 position, Quat rotation, Vec3 scale) :
		position(position),
		rotation(rotation),
		scale(scale)
	{}

	Vec3 position;
	Quat rotation;
	Vec3 scale;

	Mat4 Matrix();
};