#include "Quat.h"
#include "Math.h"

namespace
{
	Quat QuatQuatMultiply(const Quat& b, const Quat& a)
	{
		// b * a will concatenate the rotations
		// result is first (a), then (b)
		// (r1, v1)(r2, v2) = (r1r2 - v1.v2, r1v2 + r2v1 + v1 x v2)
		return Quat(
			b.w * a.x + b.x * a.w - b.y * a.z + b.z * a.y,
			b.w * a.y + b.x * a.z + b.y * a.w - b.z * a.x,
			b.w * a.z - b.x * a.y + b.y * a.x + b.z * a.w,
			b.w * a.w - b.x * a.x - b.y * a.y - b.z * a.z
		);
	}
}

const Quat Quat::zero = Quat(0.f, 0.f, 0.f, 0.f);
const Quat Quat::identity = Quat(0.f, 0.f, 0.f, 1.f);

Quat Quat::AngleAxis(float angle, const Vec3& axis)
{
	float half_a = angle / 2.f;
	float s, c;
	Math::SinCos(half_a, s, c);

	return Quat(
		axis.x * s,
		axis.y * s,
		axis.z * s,
		c
	);
}

bool Quat::NearlyZero(const Quat& q, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return Math::NearlyZero(q.x, margin) &&
		Math::NearlyZero(q.y, margin) &&
		Math::NearlyZero(q.z, margin) &&
		Math::NearlyZero(q.w, margin);
}

bool Quat::NearlyEquals(const Quat& a, const Quat& b, const float margin /*= KINDA_SMALL_NUMBER*/)
{
	return Math::NearlyEquals(a.x, b.x, margin) &&
		Math::NearlyEquals(a.y, b.y, margin) &&
		Math::NearlyEquals(a.z, b.z, margin) &&
		Math::NearlyEquals(a.w, b.w, margin);
}

bool Quat::Isidentity(const Quat& q)
{
	return NearlyEquals(q, identity);
}

bool Quat::IsUnit(const Quat& q)
{
	return Math::NearlyEquals(LengthSqrd(q), 1.f);
}

float Quat::LengthSqrd(const Quat& q)
{
	return q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
}

float Quat::Length(const Quat& q)
{
	return Math::Sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

Quat Quat::Normalize(const Quat& q)
{
	float len_inv = 1.f / Math::Sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
	return Quat(
		q.x * len_inv,
		q.y * len_inv,
		q.z * len_inv,
		q.w * len_inv
	);
}

Quat Quat::operator*(const Quat& rhs) const
{
	return QuatQuatMultiply(*this, rhs);
}

Quat& Quat::operator*=(const Quat& rhs)
{
	*this = QuatQuatMultiply(*this, rhs);
	return *this;
}

Quat Quat::operator*(float scalar) const
{
	return Quat(
		x * scalar,
		y * scalar,
		z * scalar,
		w * scalar
	);
}
Quat& Quat::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;

	return *this;
}
Quat Quat::operator/(float scalar) const
{
	return Quat(
		x / scalar,
		y / scalar,
		z / scalar,
		w / scalar
	);
}
Quat& Quat::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;

	return *this;
}

Vec3 Quat::operator*(const Vec3& v) const
{
	// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
	// V' = V + 2w(Q x V) + (2Q x (Q x V))
	// refactor:
	// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
	// T = 2(Q x V);
	// V' = V + w*(T) + (Q x T)

	const Vec3 q(x, y, z);
	const Vec3 u = Vec::Cross(q, v);
	const Vec3 a = Vec::Cross(q, u) * 2.f;
	const Vec3 b = u * 2.f * w;

	return v + a + b;
}

Vec4 Quat::operator*(const Vec4& vec) const
{
	return Vec4();
}

float Quat::Angle() const
{
	return Math::Acos(w) * 2.f;
}

Vec3 Quat::Axis() const
{
	Vec3 v(x, y, z);

	// zero-rotation, undefined axis
	if (Vec::NearlyZero(v))
		return v;

	return Vec::Normalize(v);
}

Mat4 Quat::Matrix() const
{
	// Copied from Unreal! Read up on this!
	Mat4 m;
	const float x2 = x + x;		const float y2 = y + y;			const float z2 = z + z;
	const float xx = x * x2;	const float xy = x * y2;		const float xz = x * z2;
	const float yy = y * y2;	const float yz = y * z2;		const float zz = z * z2;
	const float wx = w * x2;	const float wy = w * y2;		const float wz = w * z2;

	m.m00 = 1.0f - (yy + zz);	m.m10 = xy - wz;				m.m20 = xz + wy;			m.m30 = 0.f;
	m.m01 = xy + wz;			m.m11 = 1.0f - (xx + zz);		m.m21 = yz - wx;			m.m31 = 0.f;
	m.m02 = xz - wy;			m.m12 = yz + wx;				m.m22 = 1.0f - (xx + yy);	m.m32 = 0.f;
	m.m03 = 0.0f;				m.m13 = 0.0f;					m.m23 = 0.0f;				m.m33 = 1.0f;

	return m;
}

Quat Quat::Conjugate(const Quat& q)
{
	return Quat(-q.x, -q.y, -q.z, q.w);
}

Quat Quat::FromToQuat(const Quat& from, const Quat& to)
{
	// diff * from = to
	// diff = to * inverse(from)
	return Quat::Normalize(to * Quat::Conjugate(from));
}