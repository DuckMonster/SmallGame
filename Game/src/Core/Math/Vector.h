#pragma once
#include "Vector/Vec2.h"
#include "Vector/Vec3.h"
#include "Vector/Vec4.h"

// All common vector functions are contained in this static class,
//		to avoid code being so scattered
class Vec
{
public:

	//Returns if the input is almost-or-equal to zero with some error margin
	static bool NearlyZero(const Vec2& v, const float margin = KINDA_SMALL_NUMBER);
	static bool NearlyZero(const Vec3& v, const float margin = KINDA_SMALL_NUMBER);
	static bool NearlyZero(const Vec4& v, const float margin = KINDA_SMALL_NUMBER);

	// Returns if two vectors are almost equal to each or with some error margin
	static bool NearlyEquals(const Vec2& a, const Vec2& b, const float margin = KINDA_SMALL_NUMBER);
	static bool NearlyEquals(const Vec3& a, const Vec3& b, const float margin = KINDA_SMALL_NUMBER);
	static bool NearlyEquals(const Vec4& a, const Vec4& b, const float margin = KINDA_SMALL_NUMBER);

	// Returns the squared length (or magnitude) of the input vector
	static float LengthSqrd(const Vec2& v);
	static float LengthSqrd(const Vec3& v);
	static float LengthSqrd(const Vec4& v);

	// Returns the absolute length (or magnitude) of the input vector
	static float Length(const Vec2& v);
	static float Length(const Vec3& v);
	static float Length(const Vec4& v);

	// Returns a normalized copy of the input vector
	static Vec2 Normalize(const Vec2& v);
	static Vec3 Normalize(const Vec3& v);
	static Vec4 Normalize(const Vec4& v);

	// Returns the dot-product of two vectors (a * b)
	static float Dot(const Vec2& a, const Vec2& b);
	static float Dot(const Vec3& a, const Vec3& b);
	static float Dot(const Vec4& a, const Vec4& b);

	// Returns the squared distance between two vectors
	static float DistanceSqrd(const Vec2& a, const Vec2& b);
	static float DistanceSqrd(const Vec3& a, const Vec3& b);
	static float DistanceSqrd(const Vec4& a, const Vec4& b);

	// Returns the absolute distance between two vectors
	static float Distance(const Vec2& a, const Vec2& b);
	static float Distance(const Vec3& a, const Vec3& b);
	static float Distance(const Vec4& a, const Vec4& b);

	// Interpolates between a and b with time t
	static Vec2 Lerp(const Vec2& a, const Vec2& b, float t);
	static Vec3 Lerp(const Vec3& a, const Vec3& b, float t);
	static Vec4 Lerp(const Vec4& a, const Vec4& b, float t);

	// Returns the cross-product of a and b (a x b)
	static Vec3 Cross(const Vec3& lhs, const Vec3& rhs);

	// Clamps a vector between two points (clamps each element separately)
	static Vec2 Clamp(const Vec2& v, const Vec2& min, const Vec2& max);
	static Vec3 Clamp(const Vec3& v, const Vec3& min, const Vec3& max);
	static Vec4 Clamp(const Vec4& v, const Vec4& min, const Vec4& max);

	// Contrains a vector to another vector (assumes that constainVec is normalized)
	static Vec2 ContrainToVector(const Vec2& v, const Vec2& constrainVec);
	static Vec3 ContrainToVector(const Vec3& v, const Vec3& constrainVec);
	static Vec4 ContrainToVector(const Vec4& v, const Vec4& constrainVec);

	// Contrains a vector to a plane (assumes that planeNormal is normalized)
	static Vec2 ConstrainToPlane(const Vec2& v, const Vec2& normal);
	static Vec3 ConstrainToPlane(const Vec3& v, const Vec3& normal);
	static Vec4 ConstrainToPlane(const Vec4& v, const Vec4& normal);
};