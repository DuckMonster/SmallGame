#pragma once
struct Vec3;
struct Vec4;

struct Vec2
{
	// Constant values
	const static Vec2 zero;
	const static Vec2 one;

	const static Vec2 axis_x;
	const static Vec2 axis_y;

	Vec2() : x(0.f), y(0.f) {}
	Vec2(float scalar) :
		x(scalar), y(scalar) {}
	Vec2(float x, float y) :
		x(x), y(y) {}
	Vec2(const Vec3& vec);
	Vec2(const Vec4& vec);

	// Vector - vector operators
	Vec2	operator+(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
	Vec2&	operator+=(const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	Vec2	operator-(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
	Vec2&	operator-=(const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	Vec2	operator*(const Vec2& rhs) const { return Vec2(x * rhs.x, y * rhs.y); }
	Vec2&	operator*=(const Vec2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
	}
	Vec2	operator/(const Vec2& rhs) const { return Vec2(x / rhs.x, y / rhs.y); }
	Vec2&	operator/=(const Vec2& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
	}

	// Vector - float operators
	Vec2	operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
	Vec2&	operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}
	Vec2	operator/(float scalar) const { return Vec2(x / scalar, y / scalar); }
	Vec2&	operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	// Unary operators
	Vec2		operator-() const { return Vec2(-x, -y); };
	Vec2		operator+() const { return *this; }

	// Array-operator (mostly for use in matrices and such)
	float& operator[](uint32 index)
	{
#ifdef DEBUG
		if (index >= 2)
			Error("Vector [] operator out of bounds");
#endif
		return ((float*)this)[index];
	}
	const float& operator[](uint32 index) const
	{
#ifdef DEBUG
		if (index >= 2)
			Error("Vector [] operator out of bounds");
#endif
		return ((float*)this)[index];
	}

	float x;
	float y;
};
