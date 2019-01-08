#pragma once
#include "Constants.h"

class Math
{
public:
	// Finds the shortest difference between two angles (degrees)
	static float GetDeltaAngleDegrees(float from, float to);
	// Finds the shortest difference between two angles (radians)
	static float GetDeltaAngleRadians(float from, float to);

	// Wraps a value between two ranges
	template<typename T>
	static float Wrap(T val, T min, T max);

	// Helper function to wrap an angle to [-180, 180]
	static float UnwindDegrees(float angle);

	// Helper function to wrap an angle to [-pi, pi]
	static float UnwindRadians(float angle);

	// Logarithmic functions
	static float Ln(float v);
	static float Log10(float v);
	static float Log2(float v);

	// Round functions
	static float Round(float v);
	static float Ceil(float v);
	static float Floor(float v);
	static int RoundToInt(float v);
	static int CeilToInt(float v);
	static int FloorToInt(float v);

	// Exponential functions
	static float Pow(float v, float exp);
	static int Pow(int v, int exp);
	static float Sqrt(float v);

	// Returns the absolute (positive) value of v
	static float Abs(float v);
	static int32 Abs(int32 v);

	// Returns the sign of v (+1 if its position, -1 if its negative, 0 otherwise)
	static float Sign(float v);
	static int32 Sign(int32 v);

	// Find closest power of two from some integer
	static uint32 FindClosestPo2(uint32 value);

	// Find closest larger power of two from some integer
	static uint32 FindClosestLargerPo2(uint32 value);

	// Find closest smaller power of two from some interger
	static uint32 FindClosestSmallerPo2(uint32 value);

	// Checks if two floats almost equal each other
	static bool NearlyEquals(float a, float b, float margin = KINDA_SMALL_NUMBER);

	// Check if a float is nearly zero
	static bool NearlyZero(float a, const float margin = KINDA_SMALL_NUMBER);

	// Trigonometric
	// Sine of a
	static float Sin(float a);
	// Cosine of a
	static float Cos(float a);
	// Tangent of a
	static float Tan(float a);
	// Arc-sine of v
	static float Asin(float v);
	// Arc-cosine of v
	static float Acos(float v);
	// Arc-tangent of v
	static float Atan(float v);
	// Atan2 of y,x
	static float Atan2(float y, float x);

	// Returns both sin and cosine of some angle
	static void SinCos(float a, float& sin_a, float& cos_a);

	// Converts degrees to radians
	static float Radians(float deg);

	// Converts radians to degrees
	static float Degrees(float rad);

	// Clamps a value between a min and max
	template<typename T>
	static T Clamp(T v, T min, T max);

	// Quick clamp between 0.f-1.f
	static float Saturate(float v) { return Clamp(v, 0.f, 1.f); }

	// Gets the smaller amount of two values
	template<typename T>
	static T Min(T a, T b);

	// Gets the larger amount of two values
	template<typename T>
	static T Max(T a, T b);
};

#include "Math.inl"
