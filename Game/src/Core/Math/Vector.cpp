#include "Vector.h"

//Returns if the input is almost-or-equal to zero with some error margin
bool Vec::NearlyZero(const Vec2& v, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return Math::NearlyZero(v.x, margin) &&
		Math::NearlyZero(v.y, margin);
}
bool Vec::NearlyZero(const Vec3& v, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return Math::NearlyZero(v.x, margin) &&
		Math::NearlyZero(v.y, margin) &&
		Math::NearlyZero(v.z, margin);
}
bool Vec::NearlyZero(const Vec4& v, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return Math::NearlyZero(v.x, margin) &&
		Math::NearlyZero(v.y, margin) &&
		Math::NearlyZero(v.z, margin) &&
		Math::NearlyZero(v.w, margin);
}

// Returns if two vectors are almost equal to each or with some error margin
bool Vec::NearlyEquals(const Vec2& a, const Vec2& b, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return Math::NearlyEquals(a.x, b.x, margin) &&
		Math::NearlyEquals(a.y, b.y, margin);
}
bool Vec::NearlyEquals(const Vec3& a, const Vec3& b, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return Math::NearlyEquals(a.x, b.x, margin) &&
		Math::NearlyEquals(a.y, b.y, margin) &&
		Math::NearlyEquals(a.z, b.z, margin);
}
bool Vec::NearlyEquals(const Vec4& a, const Vec4& b, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return Math::NearlyEquals(a.x, b.x, margin) &&
		Math::NearlyEquals(a.y, b.y, margin) &&
		Math::NearlyEquals(a.z, b.z, margin) &&
		Math::NearlyEquals(a.w, b.w, margin);
}

// Returns the squared length (or magnitude) of the input vector
float Vec::LengthSqrd(const Vec2& v)
{
	return v.x * v.x + v.y * v.y;
}
float Vec::LengthSqrd(const Vec3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
float Vec::LengthSqrd(const Vec4& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

// Returns the absolute length (or magnitude) of the input vector
float Vec::Length(const Vec2& v)
{
	return Math::Sqrt(v.x * v.x + v.y * v.y);
}
float Vec::Length(const Vec3& v)
{
	return Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
float Vec::Length(const Vec4& v)
{
	return Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

// Returns a normalized copy of the input vector
Vec2 Vec::Normalize(const Vec2& v)
{
	float len = v.x * v.x + v.y * v.y;
	// In case length is almost zero, just return zero vector
	if (len < KINDA_SMALL_NUMBER)
		return Vec2::Zero;

	len = Math::Sqrt(len); 
	return Vec2(
		v.x / len,
		v.y / len
	);
}
Vec3 Vec::Normalize(const Vec3& v)
{
	float len = v.x * v.x + v.y * v.y + v.z * v.z;
	// In case length is almost zero, just return zero vector
	if (len < KINDA_SMALL_NUMBER)
		return Vec3::Zero;

	len = Math::Sqrt(len); 
	return Vec3(
		v.x / len,
		v.y / len,
		v.z / len
	);
}
Vec4 Vec::Normalize(const Vec4& v)
{
	float len = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	// In case length is almost zero, just return zero vector
	if (len < KINDA_SMALL_NUMBER)
		return Vec4::Zero;

	len = Math::Sqrt(len); 
	return Vec4(
		v.x / len,
		v.y / len,
		v.z / len,
		v.w / len
	);
}

// Returns the dot-product of two vectors (a * b)
float Vec::Dot(const Vec2& a, const Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}
float Vec::Dot(const Vec3& a, const Vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
float Vec::Dot(const Vec4& a, const Vec4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Returns the squared distance between two vectors
float Vec::DistanceSqrd(const Vec2& a, const Vec2& b)
{
	return LengthSqrd(b - a);
}
float Vec::DistanceSqrd(const Vec3& a, const Vec3& b)
{
	return LengthSqrd(b - a);
}
float Vec::DistanceSqrd(const Vec4& a, const Vec4& b)
{
	return LengthSqrd(b - a);
}

// Returns the absolute distance between two vectors
float Vec::Distance(const Vec2& a, const Vec2& b)
{
	return Length(b - a);
}
float Vec::Distance(const Vec3& a, const Vec3& b)
{
	return Length(b - a);
}
float Vec::Distance(const Vec4& a, const Vec4& b)
{
	return Length(b - a);
}

// Interpolates between a and b with time t
Vec2 Vec::Lerp(const Vec2& a, const Vec2& b, float t)
{
	return a + (b - a) * t;
}

Vec3 Vec::Lerp(const Vec3& a, const Vec3& b, float t)
{
	return a + (b - a) * t;
}

Vec4 Vec::Lerp(const Vec4& a, const Vec4& b, float t)
{
	return a + (b - a) * t;
}

// Returns the cross-product of a and b (a x b)
Vec3 Vec::Cross(const Vec3& lhs, const Vec3& rhs)
{
	return Vec3(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
	);
}

// Clamps a vector between two points (clamps each element separately)
Vec2 Vec::Clamp(const Vec2& v, const Vec2& min, const Vec2& max)
{
	return Vec2(
		Math::Clamp(v.x, min.x, max.x),
		Math::Clamp(v.y, min.y, max.y)
	);
}
Vec3 Vec::Clamp(const Vec3& v, const Vec3& min, const Vec3& max)
{
	return Vec3(
		Math::Clamp(v.x, min.x, max.x),
		Math::Clamp(v.y, min.y, max.y),
		Math::Clamp(v.z, min.z, max.z)
	);
}
Vec4 Vec::Clamp(const Vec4& v, const Vec4& min, const Vec4& max)
{
	return Vec4(
		Math::Clamp(v.x, min.x, max.x),
		Math::Clamp(v.y, min.y, max.y),
		Math::Clamp(v.z, min.z, max.z),
		Math::Clamp(v.w, min.w, max.w)
	);
}

// Contrains a vector to another vector (assumes that constain_vec is normalized)
Vec2 Vec::ContrainToVector(const Vec2& v, const Vec2& constrain_vec)
{
	return constrain_vec * Vec::Dot(v, constrain_vec);
}
Vec3 Vec::ContrainToVector(const Vec3& v, const Vec3& constrain_vec)
{
	return constrain_vec * Vec::Dot(v, constrain_vec);
}
Vec4 Vec::ContrainToVector(const Vec4& v, const Vec4& constrain_vec)
{
	return constrain_vec * Vec::Dot(v, constrain_vec);
}

// Contrains a vector to a plane (assumes that normal is normalized)
Vec2 Vec::ConstrainToPlane(const Vec2& v, const Vec2& normal)
{
	return v - (normal * Vec::Dot(v, normal));
}
Vec3 Vec::ConstrainToPlane(const Vec3& v, const Vec3& normal)
{
	return v - (normal * Vec::Dot(v, normal));
}
Vec4 Vec::ConstrainToPlane(const Vec4& v, const Vec4& normal)
{
	return v - (normal * Vec::Dot(v, normal));
}