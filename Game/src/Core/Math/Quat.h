#pragma once
#include "Matrix.h"
class Vec3;
class Vec4;

class Quat
{
public:
	// Constant quaternions
	static const Quat zero;
	static const Quat identity;

	// Returns a quaternion representing a rotation around and axis (ASSUMES axis IS NORMALIZED)
	static Quat AngleAxis(float angle, const Vec3& axis);

	static bool NearlyZero(const Quat& q, const float margin = KINDA_SMALL_NUMBER);
	static bool NearlyEquals(const Quat& a, const Quat& b, const float margin = KINDA_SMALL_NUMBER);
	static bool Isidentity(const Quat& q);
	static bool IsUnit(const Quat& q);

	static float LengthSqrd(const Quat& q);
	static float Length(const Quat& q);
	static Quat Normalize(const Quat& q);

	static Quat Conjugate(const Quat& q);

	static Quat FromToQuat(const Quat& from, const Quat& to);

	Quat() : x(0.f), y(0.f), z(0.f), w(1.f) {}
	Quat(float x, float y, float z, float w) :
		x(x), y(y), z(z), w(w) {}

	Quat operator*(const Quat& rhs) const;
	Quat& operator*=(const Quat& rhs);

	// Scalar operators
	Quat operator*(float scalar) const;
	Quat& operator*=(float scalar);
	Quat operator/(float scalar) const;
	Quat& operator/=(float scalar);

	Vec3 operator*(const Vec3& vec) const;
	Vec4 operator*(const Vec4& vec) const;

	// Returns the rotation angle of this quaternion
	float Angle() const;
	// Return the rotation axis of this quaternion
	Vec3 Axis() const;

	// Converts quaternion into a matrix
	Mat4 Matrix() const;

	float x;
	float y;
	float z;
	float w;
};
