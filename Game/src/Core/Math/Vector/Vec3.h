#pragma once
struct Vec2;
struct Vec4;

struct Vec3
{
	// Static vector constants
	const static Vec3 zero;
	const static Vec3 one;

	const static Vec3 axis_x;
	const static Vec3 axis_y;
	const static Vec3 axis_z;

	const static Vec3 forward;
	const static Vec3 up;
	const static Vec3 right;

	Vec3() : x(0.f), y(0.f), z(0.f) {}
	Vec3(float scalar) :
		x(scalar), y(scalar), z(scalar) {}
	Vec3(float x, float y, float z) :
		x(x), y(y), z(z) {}
	Vec3(const Vec2& vec, float z);
	Vec3(const Vec4& vec);

	// Vector - vector operators
	Vec3		operator+(const Vec3& rhs) const { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vec3&	operator+=(const Vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vec3		operator-(const Vec3& rhs) const { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vec3&	operator-=(const Vec3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	Vec3		operator*(const Vec3& rhs) const { return Vec3(x * rhs.x, y * rhs.y, z * rhs.z); }
	Vec3&	operator*=(const Vec3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	Vec3		operator/(const Vec3& rhs) const { return Vec3(x / rhs.x, y / rhs.y, z / rhs.z); }
	Vec3&	operator/=(const Vec3& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}

	// Vector - float operators
	Vec3		operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
	Vec3&	operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	Vec3		operator/(float scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }
	Vec3&	operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	// Unary operators
	Vec3		operator-() const { return Vec3(-x, -y, -z); };
	Vec3		operator+() const { return *this; }

	// Array-operator (mostly for use in matrices and such)
	float& operator[](uint32 index)
	{
#ifdef DEBUG
		if (index >= 3)
			Error("Vector [] operator out of bounds");
#endif
		return ((float*)this)[index];
	}
	const float& operator[](uint32 index) const
	{
#ifdef DEBUG
		if (index >= 3)
			Error("Vector [] operator out of bounds");
#endif
		return ((float*)this)[index];
	}

	float x;
	float y;
	float z;
};
