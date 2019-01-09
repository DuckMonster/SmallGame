#pragma once
#include "Core/Render/Material.h"
#include "Core/Render/Mesh.h"

class DebugDrawManager
{
	struct Cube
	{
		Mat4 mat;
		Color color;
	};

	struct Sphere
	{
		Vec3 origin;
		float radius;
		Color color;
	};

	struct Line
	{
		struct Vertex
		{
			Vec3 position;
			Color color;
		};

		Vertex vertices[2];
	};

public:
	DebugDrawManager();

	void AddCube(const Mat4& mat, const Color& color);
	void AddSphere(const Vec3& origin, float radius, const Color& color);
	void AddLine(const Vec3& start, const Vec3& end, const Color& color);

	void DrawAndClear(const Mat4& camera);

private:
	Array<Cube> cubes;
	Array<Sphere> spheres;
	Array<Line> lines;

	Material* mat_cube;
	Material* mat_sphere;
	Material* mat_line;

	Mesh* mesh_cube;
	Mesh* mesh_sphere;
	Mesh mesh_cube_lines;
	Mesh mesh_sphere_lines;
	Mesh mesh_line;
};

extern DebugDrawManager* gDebugDrawManager;
