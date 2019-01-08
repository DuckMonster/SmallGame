#include "Matrix.h"

namespace
{
	Mat4 MatMatMultiply(const Mat4& a, const Mat4& b)
	{
		// Matrix multiplication is column-major
		// So for A x B, each element [i, j] will be dot(A.column[j], B.row[i])

		return Mat4(
			// a[0-3, 0] * b[0, 0-3]
			a.m00 * b.m00 + a.m10 * b.m01 + a.m20 * b.m02 + a.m30 * b.m03,
			// a[0-3, 1] * b[0, 0-3]
			a.m01 * b.m00 + a.m11 * b.m01 + a.m21 * b.m02 + a.m31 * b.m03,
			// a[0-3, 2] * b[0, 0-3]
			a.m02 * b.m00 + a.m12 * b.m01 + a.m22 * b.m02 + a.m32 * b.m03,
			// a[0-3, 3] * b[0, 0-3]
			a.m03 * b.m00 + a.m13 * b.m01 + a.m23 * b.m02 + a.m33 * b.m03,

			// a[0-3, 0] * b[1, 0-3]
			a.m00 * b.m10 + a.m10 * b.m11 + a.m20 * b.m12 + a.m30 * b.m13,
			// a[0-3, 1] * b[1, 0-3]
			a.m01 * b.m10 + a.m11 * b.m11 + a.m21 * b.m12 + a.m31 * b.m13,
			// a[0-3, 2] * b[1, 0-3]
			a.m02 * b.m10 + a.m12 * b.m11 + a.m22 * b.m12 + a.m32 * b.m13,
			// a[0-3, 3] * b[1, 0-3]
			a.m03 * b.m10 + a.m13 * b.m11 + a.m23 * b.m12 + a.m33 * b.m13,

			// a[0-3, 0] * b[2, 0-3]
			a.m00 * b.m20 + a.m10 * b.m21 + a.m20 * b.m22 + a.m30 * b.m23,
			// a[0-3, 1] * b[2, 0-3]
			a.m01 * b.m20 + a.m11 * b.m21 + a.m21 * b.m22 + a.m31 * b.m23,
			// a[0-3, 2] * b[2, 0-3]
			a.m02 * b.m20 + a.m12 * b.m21 + a.m22 * b.m22 + a.m32 * b.m23,
			// a[0-3, 3] * b[2, 0-3]
			a.m03 * b.m20 + a.m13 * b.m21 + a.m23 * b.m22 + a.m33 * b.m23,

			// a[0-3, 0] * b[3, 0-3]
			a.m00 * b.m30 + a.m10 * b.m31 + a.m20 * b.m32 + a.m30 * b.m33,
			// a[0-3, 1] * b[3, 0-3]
			a.m01 * b.m30 + a.m11 * b.m31 + a.m21 * b.m32 + a.m31 * b.m33,
			// a[0-3, 2] * b[3, 0-3]
			a.m02 * b.m30 + a.m12 * b.m31 + a.m22 * b.m32 + a.m32 * b.m33,
			// a[0-3, 3] * b[3, 0-3]
			a.m03 * b.m30 + a.m13 * b.m31 + a.m23 * b.m32 + a.m33 * b.m33
		);
	}

	void MatInverse(float* dst, float* src)
	{
		// Copied straight from Unreal! Read up on this someday.
		const float s0  = (float)(src[ 0]); const float s1  = (float)(src[ 1]); const float s2  = (float)(src[ 2]); const float s3  = (float)(src[ 3]);
		const float s4  = (float)(src[ 4]); const float s5  = (float)(src[ 5]); const float s6  = (float)(src[ 6]); const float s7  = (float)(src[ 7]);
		const float s8  = (float)(src[ 8]); const float s9  = (float)(src[ 9]); const float s10 = (float)(src[10]); const float s11 = (float)(src[11]);
		const float s12 = (float)(src[12]); const float s13 = (float)(src[13]); const float s14 = (float)(src[14]); const float s15 = (float)(src[15]);

		float inv[16];
		inv[0]  =  s5 * s10 * s15 - s5 * s11 * s14 - s9 * s6 * s15 + s9 * s7 * s14 + s13 * s6 * s11 - s13 * s7 * s10;
		inv[1]  = -s1 * s10 * s15 + s1 * s11 * s14 + s9 * s2 * s15 - s9 * s3 * s14 - s13 * s2 * s11 + s13 * s3 * s10;
		inv[2]  =  s1 * s6  * s15 - s1 * s7  * s14 - s5 * s2 * s15 + s5 * s3 * s14 + s13 * s2 * s7  - s13 * s3 * s6;
		inv[3]  = -s1 * s6  * s11 + s1 * s7  * s10 + s5 * s2 * s11 - s5 * s3 * s10 - s9  * s2 * s7  + s9  * s3 * s6;
		inv[4]  = -s4 * s10 * s15 + s4 * s11 * s14 + s8 * s6 * s15 - s8 * s7 * s14 - s12 * s6 * s11 + s12 * s7 * s10;
		inv[5]  =  s0 * s10 * s15 - s0 * s11 * s14 - s8 * s2 * s15 + s8 * s3 * s14 + s12 * s2 * s11 - s12 * s3 * s10;
		inv[6]  = -s0 * s6  * s15 + s0 * s7  * s14 + s4 * s2 * s15 - s4 * s3 * s14 - s12 * s2 * s7  + s12 * s3 * s6;
		inv[7]  =  s0 * s6  * s11 - s0 * s7  * s10 - s4 * s2 * s11 + s4 * s3 * s10 + s8  * s2 * s7  - s8  * s3 * s6;
		inv[8]  =  s4 * s9  * s15 - s4 * s11 * s13 - s8 * s5 * s15 + s8 * s7 * s13 + s12 * s5 * s11 - s12 * s7 * s9;
		inv[9]  = -s0 * s9  * s15 + s0 * s11 * s13 + s8 * s1 * s15 - s8 * s3 * s13 - s12 * s1 * s11 + s12 * s3 * s9;
		inv[10] =  s0 * s5  * s15 - s0 * s7  * s13 - s4 * s1 * s15 + s4 * s3 * s13 + s12 * s1 * s7  - s12 * s3 * s5;
		inv[11] = -s0 * s5  * s11 + s0 * s7  * s9  + s4 * s1 * s11 - s4 * s3 * s9  - s8  * s1 * s7  + s8  * s3 * s5;
		inv[12] = -s4 * s9  * s14 + s4 * s10 * s13 + s8 * s5 * s14 - s8 * s6 * s13 - s12 * s5 * s10 + s12 * s6 * s9;
		inv[13] =  s0 * s9  * s14 - s0 * s10 * s13 - s8 * s1 * s14 + s8 * s2 * s13 + s12 * s1 * s10 - s12 * s2 * s9;
		inv[14] = -s0 * s5  * s14 + s0 * s6  * s13 + s4 * s1 * s14 - s4 * s2 * s13 - s12 * s1 * s6  + s12 * s2 * s5;
		inv[15] =  s0 * s5  * s10 - s0 * s6  * s9  - s4 * s1 * s10 + s4 * s2 * s9  + s8  * s1 * s6  - s8  * s2 * s5;

		float det = s0 * inv[0] + s1 * inv[4] + s2 * inv[8] + s3 * inv[12];
		if( det != 0.0 )
		{
			det = 1.0 / det;
		}
		for( int i = 0; i < 16; i++ )
		{
			dst[i] = inv[i] * det;
		}
	}
}

const Mat4 Mat4::Identity = Mat4(
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
);

Mat4 Mat4::Scale(const Vec3& scale)
{
	Mat4 result(1.f);
	result.SetDiagonal(Vec4(scale, 1.f));

	return result;
}

Mat4 Mat4::Translation(const Vec3& translation)
{
	Mat4 result(1.f);
	result.GetRow(3) = Vec4(translation, 1.f);

	return result;
}

Mat4 Mat4::Rotation(const Quat& rotation)
{
	return rotation.Matrix();
}

Mat4 Mat4::TranslateRotate(const Vec3& translation, const Quat& rotation)
{
	// @Optimize
	return Translation(translation) * Rotation(rotation);
}

Mat4 Mat4::TranslateRotateScale(const Vec3& translation, const Quat& rotation, const Vec3& scale)
{
	// @Optimize
	return Translation(translation) * Rotation(rotation) * Scale(scale);
}

Mat4 Mat4::Perspective(float fov, float aspect, float near, float far)
{
	// Look this up :(
	const float tanHalfFov = Math::Tan(fov / 2.f);
	return Mat4(
		1.f / (aspect * tanHalfFov),			0.f, 0.f, 0.f,
		0.f, 1.f / tanHalfFov,						 0.f, 0.f,
		0.f, 0.f, -((far + near) / (far - near)),		 -1.f,
		0.f, 0.f, -((2.f * far * near) / (far - near)),	  0.f
	);
}

Mat4 Mat4::Ortho(float left, float right, float bottom, float top, float near, float far)
{
	// Scale x by the half-inverse width of the ortho (width is right - left)
	//		so half-inverse if (2 / (right - left))
	// It's half, because the width is the _diameter_, not the _radius_

	// Same with y and z
	// Since we can't assume it will be centered (left = -right etc.), we also have to translate everything
	//		so that left always becomes -1.0 and right 1.0 etc.
	// Easiest way would be to pre-translate by the center of the two before scaling

	//			  ___ center __________   __ 2 / width _______
	// x' = (x + -((right - left) / 2)) * (2 / (right - left))

	// However, translation always happens _together_ with scaling, so the actual formula would be:
	// x' = x * (2 / (right - left)) + -((right - left) / 2)
	// Which is no good

	// However, we can simply factor out the first formula
	// t = translation
	// s = scaling
	// x' = (x + t) * s
	// x' = s * x + t * x

	// And
	//	 2		  r + l
	// -----  *	- -----
	// r - l	    2

	// Equals
	//	 r + l
	// - -----
	//	 r - l

	// So the final formula 
	// x' = 2 / (r - l) + -((r + l) / (r - l))
	
	// And the same goes for all dimensions

	return Mat4(
		2.f / (right - left), 0.f, 0.f, 0.f,
		0.f, 2.f / (top - bottom), 0.f, 0.f,
		0.f, 0.f, -2.f / (far - near), 0.f,
		-((right + left) / (right - left)), -((top + bottom) / (top - bottom)), -((far + near) / (far - near)), 1.f
	);
}

Mat4 Mat4::LookAt(const Vec3& eye, const Vec3& target, const Vec3& up)
{
	// Basic look-at formula
	// 1. Calculate the basis vectors for the camera-matrix
	// 2. Construct a camera-to-world matrix by making the basis-vectors of the matrix the basis vectors
	//		we calculated
	// 3. Inverse it (since we want world-to-camera). Inversing a rotational matrix is just transposing it.
	// 4. For the translation, it will be applied as an addition to the rotated vectors, so the translations have
	//		to happen in camera-space, not world space.
	//		Therefore, we calculate the eyes' offset it _camera space_ by dotting the eye with each basis vector,
	//		which will give us how much it is offset in each of the cameras basis vectors.
	//		Inverse that (just minus it), and make that the translation of the final matrix.
	// NOTE: Do NOT inverse the Z axis, keep it as expected (+z is coming towards the viewer, or out of screen),
	//		it will get inversed by the projection matrix.

	Vec3 zaxis = Vec::Normalize(eye - target);
	Vec3 xaxis = Vec::Normalize(Vec::Cross(up, zaxis));
	Vec3 yaxis = Vec::Cross(zaxis, xaxis);

	return Mat4(
		xaxis.x, yaxis.x, zaxis.x, 0.f,
		xaxis.y, yaxis.y, zaxis.y, 0.f,
		xaxis.z, yaxis.z, zaxis.z, 0.f,
		-Vec::Dot(xaxis, eye), -Vec::Dot(yaxis, eye), -Vec::Dot(zaxis, eye), 1.f
	);
}

Mat4 Mat4::Inverse(const Mat4& mat)
{
	Mat4 result;
	MatInverse((float*)&result, (float*)&mat);
	return result;
}

Mat4 Mat4::operator*(const Mat4& other) const
{
	return MatMatMultiply(*this, other);
}

Mat4& Mat4::operator*=(const Mat4& other)
{
	*this = MatMatMultiply(*this, other);
	return *this;
}

Vec3 Mat4::operator*(const Vec3& other) const
{
	//	for(element i)
	//		for(element j)
	//			vector[i] += vector[j] * matrix[j, i]

	// Assume the w element of vector is 1, so just add row 3 at the end
	return Vec4(
		other.x * m00 + other.y * m10 + other.z * m20 + m30,
		other.x * m01 + other.y * m11 + other.z * m21 + m31,
		other.x * m02 + other.y * m12 + other.z * m22 + m32,
		other.x * m03 + other.y * m13 + other.z * m23 + m33
	);
}

Vec4 Mat4::operator*(const Vec4& other) const
{
	//	for(element i)
	//		for(element j)
	//			vector[i] += vector[j] * matrix[j, i]
	return Vec4(
		other.x * m00 + other.y * m10 + other.z * m20 + other.w * m30,
		other.x * m01 + other.y * m11 + other.z * m21 + other.w * m31,
		other.x * m02 + other.y * m12 + other.z * m22 + other.w * m32,
		other.x * m03 + other.y * m13 + other.z * m23 + other.w * m33
	);
}
