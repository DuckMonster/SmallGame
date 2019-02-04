#include "Transform.h"

const Transform Transform::identity = Transform(
	Vec3(0.f),
	Quat::identity,
	Vec3(1.f)
);

Mat4 Transform::Matrix()
{
	return Mat4::TranslateRotateScale(position, rotation, scale);
}