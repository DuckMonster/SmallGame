#include "../Test.h"

struct A
{
	void operator=(float f)
	{
	}
	void operator+(float f)
	{
	}
};

struct B : public A
{
	using A::operator=;
};

namespace
{
	bool VectorTest()
	{
		B b;
		b = 5.f;

		{
			Vec3 a, b;
			a = 5.f;
			b = 2.f;

			TEST_EXPR(Vec::NearlyEquals(a + b, Vec3(7.f)));
			TEST_EXPR(Vec::NearlyZero(a - 4.f - 1.f));
		}

		{
			Vec3 vec(5.f, 2.f, 10.f);

			float len = Vec::Length(vec);
			TEST_EXPR(Math::NearlyEquals(len, 11.3578166916f));
			vec *= 0.5f;
			len *= 0.5f;
			TEST_EXPR(Math::NearlyEquals(Vec::Length(vec), len));
			Math::NearlyEquals(0.f, 1.f);

			TEST_EXPR(Vec::NearlyZero(Vec::Normalize(Vec3::Zero)));
			Vec3 zero_vec = Vec3::Zero;

			Vec3 vec2 = Vec::Normalize(vec);
			TEST_EXPR(Math::NearlyEquals(Vec::Length(vec), len));
			TEST_EXPR(Math::NearlyEquals(Vec::Length(vec2), 1.f));
		}

		{
			TEST_EXPR(Vec::NearlyEquals(Vec::Cross(Vec3::AxisX, Vec3::AxisY), Vec3::AxisZ));
			TEST_EXPR(Vec::NearlyEquals(Vec::Cross(Vec3::AxisY, Vec3::AxisX), -Vec3::AxisZ));
			TEST_EXPR(Vec::NearlyEquals(Vec::Cross(Vec3::AxisY, Vec3::AxisZ), Vec3::AxisX));
			TEST_EXPR(Vec::NearlyEquals(Vec::Cross(Vec3::AxisZ, Vec3::AxisY), -Vec3::AxisX));
			TEST_EXPR(Vec::NearlyEquals(Vec::Cross(Vec3::AxisZ, Vec3::AxisX), Vec3::AxisY));
			TEST_EXPR(Vec::NearlyEquals(Vec::Cross(Vec3::AxisX, Vec3::AxisZ), -Vec3::AxisY));
		}

		return true;
	}

	DEFINE_TEST(VectorTest);
}