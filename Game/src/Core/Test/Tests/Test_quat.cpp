#include "../Test.h"

#define ERROR_MARGIN 0.1f

namespace
{
	bool QuatTest()
	{
		Quat q;
		TEST_EXPR(Vec::NearlyEquals(q.Axis(), Vec3::Zero));
		TEST_EXPR(q.Angle() == 0.f);

		q = Quat::AngleAxis(HALF_PI, Vec3::AxisY);
		TEST_EXPR(Vec::NearlyEquals(q.Axis(), Vec3::AxisY));
		TEST_EXPR(q.Angle() == HALF_PI);
		TEST_EXPR(Math::NearlyEquals(Math::Degrees(q.Angle()), 90.f, ERROR_MARGIN));
		TEST_EXPR(Quat::IsUnit(q));

		TEST_EXPR(Vec::NearlyEquals(q * Vec3::AxisX, -Vec3::AxisZ, ERROR_MARGIN));
		TEST_EXPR(Vec::NearlyEquals(q * Vec3::AxisY, Vec3::AxisY, ERROR_MARGIN));

		q = Quat::AngleAxis(4.92f, Vec::Normalize(Vec3(2.f, 1.f, 4.f)));
		TEST_EXPR(Vec::NearlyEquals(q * Vec3(-5.f, 0.5f, 3.f), Vec3(-1.04f, 5.75f, -0.29f), ERROR_MARGIN));

		return true;
	}

	DEFINE_TEST(QuatTest);
}