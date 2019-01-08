#include "Vec3.h"
#include "Core/Math/Math.h"
#include "Vec2.h"
#include "Vec4.h"

const Vec3 Vec3::Zero = Vec3(0.f);
const Vec3 Vec3::One = Vec3(1.f);

const Vec3 Vec3::AxisX = Vec3(1.f, 0.f, 0.f);
const Vec3 Vec3::AxisY = Vec3(0.f, 1.f, 0.f);
const Vec3 Vec3::AxisZ = Vec3(0.f, 0.f, 1.f);

const Vec3 Vec3::Forward = AxisX;
const Vec3 Vec3::Up = AxisY;
const Vec3 Vec3::Right = AxisZ;

Vec3::Vec3(const Vec2& vec, float z) :
	x(vec.x), y(vec.y), z(z) {}

Vec3::Vec3(const Vec4& vec) :
	x(vec.x), y(vec.y), z(vec.z) {}
