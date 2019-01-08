#include "Vec2.h"

#include "Vec3.h"
#include "Vec4.h"

const Vec2 Vec2::Zero = Vec2(0.f);
const Vec2 Vec2::One = Vec2(1.f);

const Vec2 Vec2::AxisX = Vec2(1.f, 0.f);
const Vec2 Vec2::AxisY = Vec2(0.f, 1.f);

Vec2::Vec2(const Vec3& vec) :
	x(vec.x), y(vec.y) {}

Vec2::Vec2(const Vec4& vec) :
	x(vec.x), y(vec.y) {}
