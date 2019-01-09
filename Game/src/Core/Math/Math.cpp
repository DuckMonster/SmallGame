
#include "Math.h"
float Math::GetDeltaAngleDegrees(float from, float to)
{
	float dif = to - from;

	// If greater than a half-turn, then a negative turn is shorter
	if (dif > 180.f)
	{
		dif -= 360.f;
	}

	// If smaller than a half-turn, a positive turn is shorter
	if (dif < -180.f)
	{
		dif += 360.f;
	}

	return dif;
}

float Math::GetDeltaAngleRadians(float from, float to)
{
	float dif = to - from;

	// If greater than a half-turn, then a negative turn is shorter
	if (dif > PI)
	{
		dif -= TAU;
	}

	// If smaller than a half-turn, a positive turn is shorter
	if (dif < -PI)
	{
		dif += TAU;
	}

	return dif;
}

float Math::UnwindDegrees(float angle)
{
	return Wrap(angle, -180.f, 180.f);
}

float Math::UnwindRadians(float angle)
{
	return Wrap(angle, -PI, PI);
}

// Natural log (log e) of v
float Math::Ln(float v)
{
	return ::logf(v);
}

// Log base 10 of v
float Math::Log10(float v)
{
	return ::log10f(v);
}

// Log base 2 of v
float Math::Log2(float v)
{
	return ::log2f(v);
}

// Rount to nearest integer
float Math::Round(float v)
{
	return ::roundf(v);
}

// Round up
float Math::Ceil(float v)
{
	return ::ceilf(v);
}

// Round down
float Math::Floor(float v)
{
	return ::floorf(v);
}

// Round to nearest integer (and cast to int)
int Math::RoundToInt(float v)
{
	return (int)::roundf(v);
}

// Round up (and cast to int)
int Math::CeilToInt(float v)
{
	return (int)::ceilf(v);
}

// Round down (and cast to int)
int Math::FloorToInt(float v)
{
	return (int)::floorf(v);
}

// Power of (float)
float Math::Pow(float v, float exp)
{
	return ::powf(v, exp);
}

// Power of (integer)
int Math::Pow(int v, int exp)
{
	return ::powl(v, exp);
}

// Square root
float Math::Sqrt(float v)
{
	return ::sqrtf(v);
}

// Returns the absolute (positive) value of v
float Math::Abs(float v)
{
	return v < 0.f ? -v : v;
}
int32 Math::Abs(int32 v)
{
	return v < 0 ? -v : v;
}

// Returns the sign of v (+1 if its position, -1 if its negative, 0 otherwise)
float Math::Sign(float v)
{
	return v < 0.f ? -1.f : (v == 0.f ? 0.f : 1.f);
}
int32 Math::Sign(int32 v)
{
	return v < 0 ? -1 : (v == 0 ? 0 : 1);
}

// Return the power of two thats closest to value
uint32 Math::FindClosestPo2(uint32 value)
{
	float log = Math::Log2(value);
	int rounded_log = Math::RoundToInt(log);
	return Math::Pow(2, rounded_log);
}

// Return the power of two thats closest, but larger than value
uint32 Math::FindClosestLargerPo2(uint32 value)
{
	float log = Math::Log2(value);
	int ceil_log = Math::CeilToInt(log);
	return Math::Pow(2, ceil_log);
}

// Return the power of two thats closest, but smaller than value
uint32 Math::FindClosestSmallerPo2(uint32 value)
{
	float log = Math::Log2(value);
	int floor_log = Math::FloorToInt(log);
	return Math::Pow(2, floor_log);
}

// Checks if two floats almost equal each other
bool Math::NearlyEquals(float a, float b, float margin/* = KINDA_SMALL_NUMBER*/)
{
	float dif = a - b;
	return (dif < margin && dif > -margin);
}

// Checks if a floating point is almost or equal to zero
bool Math::NearlyZero(float a, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return (a < margin && a > -margin);
}

// Sine of a
float Math::Sin(float a)
{
	return ::sinf(a);
}

// Cosine of a
float Math::Cos(float a)
{
	return ::cosf(a);
}

// Tangent of a
float Math::Tan(float a)
{
	return ::tanf(a);
}

// Arc-sine of v
float Math::Asin(float v)
{
	return ::asinf(v);
}

// Arc-cosine of v
float Math::Acos(float v)
{
	return ::acosf(v);
}

// Arc-tangent of v
float Math::Atan(float v)
{
	return ::atanf(v);
}

// Atan2 of y,x
float Math::Atan2(float y, float x)
{
	return ::atan2f(y, x);
}

// Returns both sin and cosine of some angle
void Math::SinCos(float a, float& sin_a, float& cos_a)
{
	sin_a = Sin(a);
	cos_a = Cos(a);
}

// Converts degrees to radians
float Math::Radians(float deg)
{
	return deg * DEG_TO_RAD;
}

// Converts radians to degrees
float Math::Degrees(float rad)
{
	return rad * RAD_TO_DEG;
}
