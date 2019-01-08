#include "../Test.h "
#include "Core/Math/Matrix.h"

namespace
{
	bool MatrixTest()
	{
		{
			Mat4 mat(1.f);
			TEST_EXPR(Vec::NearlyEquals(mat.GetDiagonal(), Vec4(1.f, 1.f, 1.f, 1.f)));

			// Translation * scale
			Vec4 s = Vec4(5.f, 4.f, -1.f, 1.f);
			Vec4 t = Vec4(20.f, -10.f, 4.2f, 1.f);

			Mat4 scale = Mat4::Scale(s);
			Mat4 translation = Mat4::Translation(t);

			Mat4 product = translation * scale;
			TEST_EXPR(Vec::NearlyEquals(product.GetDiagonal(), s));
			TEST_EXPR(Vec::NearlyEquals(product[3], t));

			product = scale * translation;
			TEST_EXPR(Vec::NearlyEquals(product.GetDiagonal(), s));
			TEST_EXPR(!Vec::NearlyEquals(product[3], t));
		}

		{
			Quat quat = Quat::AngleAxis(HALF_PI, Vec3::AxisY);
			Mat4 scale = Mat4::Scale(Vec3(4.f, 0.f, 0.f));
			Mat4 translate = Mat4::Translation(Vec3(10.f, -2.f, 0.f));

			Mat4 r = translate * quat.Matrix() * scale;
			TEST_EXPR(Vec::NearlyEquals(r.GetRow(3), Vec4(10.f, -2.f, 0.f, 1.f)));
		}

		return true;
	}

	DEFINE_TEST(MatrixTest);
}