#include "Vec3.h"
#include "Core/Math/Math.h"
#include "Vec2.h"
#include "Vec4.h"

const Vec3 Vec3::zero = Vec3(0.f);
const Vec3 Vec3::one = Vec3(1.f);

const Vec3 Vec3::axis_x = Vec3(1.f, 0.f, 0.f);
const Vec3 Vec3::axis_y = Vec3(0.f, 1.f, 0.f);
const Vec3 Vec3::axis_z = Vec3(0.f, 0.f, 1.f);

const Vec3 Vec3::forward = axis_x;
const Vec3 Vec3::up = axis_y;
const Vec3 Vec3::right = axis_z;

Vec3::Vec3(const Vec2& vec, float z) :
	x(vec.x), y(vec.y), z(z) {}

Vec3::Vec3(const Vec4& vec) :
	x(vec.x), y(vec.y), z(vec.z) {}
