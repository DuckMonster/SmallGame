#include "../Test.h "

namespace
{
	bool MathTest()
	{
		{
			// Logs
			TEST_EXPR(Math::FindClosestPo2(2000) == 2048);
			TEST_EXPR(Math::FindClosestPo2(1024) == 1024);
			TEST_EXPR(Math::FindClosestPo2(755) == 1024);
			TEST_EXPR(Math::FindClosestSmallerPo2(1024) == 1024);
			TEST_EXPR(Math::FindClosestLargerPo2(1024) == 1024);
			TEST_EXPR(Math::FindClosestSmallerPo2(1023) == 512);
			TEST_EXPR(Math::FindClosestLargerPo2(1025) == 2048);
		}
		{
			// Trigonometric
			TEST_EXPR(Math::NearlyEquals(Math::Radians(80.f), 1.39626f, 0.01f));
			TEST_EXPR(Math::NearlyEquals(Math::Radians(25.f), 0.436332f, 0.01f));
			TEST_EXPR(Math::NearlyEquals(Math::Radians(623.f), 10.8734f, 0.01f));
			TEST_EXPR(Math::NearlyEquals(Math::Radians(-200.f), -3.49066f, 0.01f));
			TEST_EXPR(Math::NearlyEquals(Math::Degrees(1.39626f), 80.f, 0.01f));
			TEST_EXPR(Math::NearlyEquals(Math::Degrees(0.436332f), 25.f, 0.01f));
			TEST_EXPR(Math::NearlyEquals(Math::Degrees(10.8734f), 623.f, 0.01f));
			TEST_EXPR(Math::NearlyEquals(Math::Degrees(-3.49066f), -200.f, 0.01f));
		}
		return true;
	}

	DEFINE_TEST(MathTest);
}