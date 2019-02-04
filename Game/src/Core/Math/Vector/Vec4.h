#pragma once

struct Vec4;
struct Vec3;

struct Vec4
{
	// Static constant vectors
	const static Vec4 zero;
	const static Vec4 one;
	const static Vec4 axis_x;
	const static Vec4 axis_y;
	const static Vec4 axis_z;
	const static Vec4 AxisW;

	Vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
	Vec4(float scalar) :
		x(scalar), y(scalar), z(scalar), w(scalar) {}
	Vec4(float x, float y, float z, float w) :
		x(x), y(y), z(z), w(w) {}
	Vec4(const Vec2& vec, float z, float w);
	Vec4(const Vec3& vec, float w);

	// Vector - vector operators
	Vec4		operator+(const Vec4& rhs) const { return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Vec4&	operator+=(const Vec4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	Vec4		operator-(const Vec4& rhs) const { return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	Vec4&	operator-=(const Vec4& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	Vec4		operator*(const Vec4& rhs) const { return Vec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
	Vec4&	operator*=(const Vec4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}
	Vec4		operator/(const Vec4& rhs) const { return Vec4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
	Vec4&	operator/=(const Vec4& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	// Vector - float operators
	Vec4		operator*(float scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
	Vec4&	operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}
	Vec4		operator/(float scalar) const { return Vec4(x / scalar, y / scalar, z / scalar, w / scalar); }
	Vec4&	operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	// Unary operators
	Vec4		operator-() const { return Vec4(-x, -y, -z, -w); };
	Vec4		operator+() const { return *this; }

	// Array accessor operators
	float& operator[](uint32 i)
	{
#ifdef DEBUG
		if (i > 4)
			Fatal("Vector access out of bounds");
#endif

		return ((float*)this)[i];
	}
	const float& operator[](uint32 i) const
	{
#ifdef DEBUG
		if (i > 4)
			Fatal("Vector access out of bounds");
#endif

		return ((float*)this)[i];
	}

	float x;
	float y;
	float z;
	float w;
};
