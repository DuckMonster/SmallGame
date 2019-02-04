#include "../Test.h"
#include "Core/Math/Random.h"

#define ERROR_MARGIN 0.1f

namespace
{
	bool QuatTest()
	{
		{
			Quat q;
			TEST_EXPR(Vec::NearlyEquals(q.Axis(), Vec3::zero));
			TEST_EXPR(q.Angle() == 0.f);

			q = Quat::AngleAxis(HALF_PI, Vec3::axis_y);
			TEST_EXPR(Vec::NearlyEquals(q.Axis(), Vec3::axis_y));
			TEST_EXPR(q.Angle() == HALF_PI);
			TEST_EXPR(Math::NearlyEquals(Math::Degrees(q.Angle()), 90.f, ERROR_MARGIN));
			TEST_EXPR(Quat::IsUnit(q));

			TEST_EXPR(Vec::NearlyEquals(q * Vec3::axis_x, -Vec3::axis_z, ERROR_MARGIN));
			TEST_EXPR(Vec::NearlyEquals(q * Vec3::axis_y, Vec3::axis_y, ERROR_MARGIN));

			q = Quat::AngleAxis(4.92f, Vec::Normalize(Vec3(2.f, 1.f, 4.f)));
			TEST_EXPR(Vec::NearlyEquals(q * Vec3(-5.f, 0.5f, 3.f), Vec3(-1.04f, 5.75f, -0.29f), ERROR_MARGIN));
		}

		{
			for(uint32 i=0; i<100; ++i)
			{
				Quat q, q_conj;
				q = Quat::AngleAxis(Random::Range(0.f, TAU), Random::UnitVec3());
				q_conj = Quat::Conjugate(q);

				TEST_EXPR_QUIET(Quat::Isidentity(q * q_conj));
			}

			for(uint32 i=0; i<100; ++i)
			{
				Quat q1, q2;
				q1 = Quat::AngleAxis(Random::Range(0.f, TAU), Random::UnitVec3());
				q2 = Quat::AngleAxis(Random::Range(0.f, TAU), Random::UnitVec3());

				Quat from_to = Quat::FromToQuat(q1, q2);
				TEST_EXPR_QUIET(Quat::IsUnit(from_to));

				TEST_EXPR_QUIET(Quat::NearlyEquals(from_to * q1, q2));
			}

			// From-to with a small difference in angle
			{
				Quat q = Quat::identity;

				Vec3 axis = Random::UnitVec3();
				float angle = Random::Range(0.f, TAU);

				for(uint32 i=0; i<100; ++i)
				{
					float variance = Random::Range(0.5f, 1.f);
					angle += variance;

					Quat q2;
					q2 = Quat::AngleAxis(angle, axis);

					Quat from_to = Quat::FromToQuat(q, q2);
					TEST_EXPR_QUIET(Quat::IsUnit(from_to));
					TEST_EXPR_QUIET(Quat::NearlyEquals(from_to * q, q2));

					q = Quat::Normalize(from_to * q);
				}
			}
		}

		{
			for(uint32 i=0; i<100; ++i)
			{
				Quat q1, q2;
				q1 = Quat::AngleAxis(Random::Range(0.f, TAU), Random::UnitVec3());
				q2 = Quat::AngleAxis(Random::Range(0.f, TAU), Random::UnitVec3());

				TEST_EXPR_QUIET(Quat::IsUnit(q1));
				TEST_EXPR_QUIET(Quat::IsUnit(q2));
				TEST_EXPR_QUIET(Quat::IsUnit(q1 * q2));
				TEST_EXPR_QUIET(Quat::IsUnit(q2 * q1));
			}
		}

		return true;
	}

	DEFINE_TEST(QuatTest);
}