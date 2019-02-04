#pragma once
#include "Vector/Vec4.h"

class Vec3;
class Quat;

struct Mat4
{
	static const Mat4 identity;

	// Creates a scaling matrix
	static Mat4 Scale(const Vec3& scale);
	// Creates a translation matrix
	static Mat4 Translation(const Vec3& translation);
	// Creates a rotation matrix
	static Mat4 Rotation(const Quat& rotation);

	// Create a matrix that contains rotation and translation
	static Mat4 TranslateRotate(const Vec3& translation, const Quat& rotation);
	static Mat4 TranslateRotateScale(const Vec3& translation, const Quat& rotation, const Vec3& scale);

	// Returns a persepctive projection matrix
	static Mat4 Perspective(float fov, float aspect, float near, float far);

	// Returns a orthographic projection matrix
	static Mat4 Ortho(float left, float right, float down, float up, float near, float far);

	// Returns a look-at matrix
	static Mat4 LookAt(const Vec3& eye, const Vec3& target, const Vec3& up);
	static Mat4 LookAtForward(const Vec3& eye, const Vec3& forward, const Vec3& up);

	// Returns the inverse of a matrix
	static Mat4 Inverse(const Mat4& mat);

	Mat4() {}
	Mat4(float s) :
		m00(s), m01(0.f), m02(0.f), m03(0.f),
		m10(0.f), m11(s), m12(0.f), m13(0.f),
		m20(0.f), m21(0.f), m22(s), m23(0.f),
		m30(0.f), m31(0.f), m32(0.f), m33(s)
	{}
	Mat4(const Vec4& diagonal) :
		m00(diagonal.x), m01(0.f), m02(0.f), m03(0.f),
		m10(0.f), m11(diagonal.y), m12(0.f), m13(0.f),
		m20(0.f), m21(0.f), m22(diagonal.z), m23(0.f),
		m30(0.f), m31(0.f), m32(0.f), m33(diagonal.w)
	{}
	Mat4(const Vec4& x, const Vec4& y, const Vec4& z, const Vec4& w) :
		m00(x.x), m01(x.y), m02(x.z), m03(x.w),
		m10(y.x), m11(y.y), m12(y.z), m13(y.w),
		m20(z.x), m21(z.y), m22(z.z), m23(z.w),
		m30(w.x), m31(w.y), m32(w.z), m33(w.w)
	{}
	Mat4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33):
		m00(m00), m01(m01), m02(m02), m03(m03),
		m10(m10), m11(m11), m12(m12), m13(m13),
		m20(m20), m21(m21), m22(m22), m23(m23),
		m30(m30), m31(m31), m32(m32), m33(m33)
	{}

	Mat4 operator*(const Mat4& other) const;
	Mat4& operator*=(const Mat4& other);
	Vec3 operator*(const Vec3& other) const;
	Vec4 operator*(const Vec4& other) const;

	// Gets row at index i
	Vec4& GetRow(uint32 i)
	{
		return ((Vec4*)this)[i];
	}

	// Gets row at index i
	const Vec4& GetRow(uint32 i) const
	{
		return ((Vec4*)this)[i];
	}

	// Gets the diagonal of the matrix
	Vec4 GetDiagonal() const
	{
		return Vec4(m00, m11, m22, m33);
	}

	// Sets the value of the diagonal
	void SetDiagonal(const Vec4& vec)
	{
		m00 = vec.x; m11 = vec.y; m22 = vec.z; m33 = vec.w;
	}

	// Returns row i
	Vec4& operator[](uint32 i)
	{
#ifdef DEBUG
		if (i > 4)
			Fatal("Matrix access out of bounds");
#endif
		return ((Vec4*)this)[i];
	}

	// Returns row i
	const Vec4& operator[](uint32 i) const
	{
#ifdef DEBUG
		if (i > 4)
			Fatal("Matrix access out of bounds");
#endif
		return ((Vec4*)this)[i];
	}

	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;
};
