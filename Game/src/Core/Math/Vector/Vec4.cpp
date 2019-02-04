#include "Vec4.h"

#include "Vec2.h"
#include "Vec3.h"

const Vec4 Vec4::zero = Vec4(0.f);
const Vec4 Vec4::one = Vec4(1.f);
const Vec4 Vec4::axis_x = Vec4(1.f, 0.f, 0.f, 0.f);
const Vec4 Vec4::axis_y = Vec4(0.f, 1.f, 0.f, 0.f);
const Vec4 Vec4::axis_z = Vec4(0.f, 0.f, 1.f, 0.f);
const Vec4 Vec4::AxisW = Vec4(0.f, 0.f, 0.f, 1.f);

Vec4::Vec4(const Vec2& vec, float z/* = 0*/, float w/* = 0*/) :
	x(vec.x), y(vec.y), z(z), w(w) {}

Vec4::Vec4(const Vec3& vec, float w/* = 0*/) :
	x(vec.x), y(vec.y), z(vec.z), w(w) {}
