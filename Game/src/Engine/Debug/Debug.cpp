
#include "Debug.h"
#include "DebugDrawManager.h"
#include "DebugPrintManager.h"

Color Debug::default_color = Color::green;

void Debug::Print(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	gPrintManager->Print(*TString::VPrintf(format, vl), 5.f);

	va_end(vl);
}

void Debug::PrintOneFrame(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	gPrintManager->Print(*TString::VPrintf(format, vl));

	va_end(vl);
}

void Debug::DrawCube(const Vec3& origin, const Vec3& size, const Color& color /* = default_color */)
{
	Mat4 mat(1.f);
	mat[0][0] = size.x;
	mat[1][1] = size.y;
	mat[2][2] = size.z;
	mat[3] = Vec4(origin, 1.f);

	DrawCube(mat, color);
}

void Debug::DrawCube(const Mat4& mat, const Color& color /* = default_color */)
{
	gDebugDrawManager->AddCube(mat, color);
}

void Debug::DrawSphere(const Vec3& origin, float radius, const Color& color /* = default_color */)
{
	gDebugDrawManager->AddSphere(origin, radius, color);
}

void Debug::DrawLine(const Vec3& start, const Vec3& end, const Color& color /* = default_color */)
{
	gDebugDrawManager->AddLine(start, end, color);
}
