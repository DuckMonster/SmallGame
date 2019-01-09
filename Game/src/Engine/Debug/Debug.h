#pragma once
class Debug
{
private:
	static Color default_color;

public:
	static void Print(const char* format, ...);
	static void PrintOneFrame(const char* format, ...);

	static void DrawCube(const Vec3& origin, const Vec3& size, const Color& color = default_color);
	static void DrawCube(const Mat4& mat, const Color& color = default_color);
	static void DrawSphere(const Vec3& origin, float radius, const Color& color = default_color);
	static void DrawLine(const Vec3& start, const Vec3& end, const Color& color = default_color);
};