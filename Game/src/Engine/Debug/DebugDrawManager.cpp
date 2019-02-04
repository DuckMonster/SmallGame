
#include "DebugDrawManager.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/MaterialResource.h"
#include "Core/Resource/MeshResource.h"

DebugDrawManager* gDebugDrawManager = nullptr;

namespace
{
	void LoadCubeLines(Mesh& mesh)
	{
		struct Line
		{
			static Line Make(
				const Vec3& a1, const Vec3& b1,
				const Vec3& a2, const Vec3& b2,
				const Vec3& c
			)
			{
				Line line;
				line.first = a1 + b1 + c;
				line.second = a2 + b2 + c;
				return line;
			}
			Vec3 first;
			Vec3 second;
		};

		Line lines[12];

		Vec3 x(0.5, 0.f, 0.f),
			y(0.f, 0.5, 0.f),
			z(0.f, 0.f, 0.5);

		lines[0] = Line::Make(x, z, -x, z, y);
		lines[1] = Line::Make(-x, z, -x, -z, y);
		lines[2] = Line::Make(-x, -z, x, -z, y);
		lines[3] = Line::Make(x, -z, x, z, y);

		lines[4] = Line::Make(x, z, -x, z, -y);
		lines[5] = Line::Make(-x, z, -x, -z, -y);
		lines[6] = Line::Make(-x, -z, x, -z, -y);
		lines[7] = Line::Make(x, -z, x, z, -y);

		lines[8] = Line::Make(x, y, x, -y, z);
		lines[9] = Line::Make(-x, y, -x, -y, z);
		lines[10] = Line::Make(x, y, x, -y, -z);
		lines[11] = Line::Make(-x, y, -x, -y, -z);

		mesh.vertex_buffer.Create(1);
		mesh.vertex_buffer.BindBuffer(0, 0, 3, 0, 0);
		mesh.vertex_buffer.BufferData(0, lines, sizeof(lines));
		mesh.draw_mode = GL_LINES;
		mesh.draw_count = 2 * 12;
		mesh.use_elements = false;
	}

	void LoadSphereLines(Mesh& mesh)
	{
		struct Line
		{
			static Line Make(float s1, float s2, float c1, float c2, const Vec3& sa, const Vec3& ca)
			{
				Line line;
				line.first = sa * s1 + ca * c1;
				line.second = sa * s2 + ca * c2;
				return line;
			}

			Vec3 first;
			Vec3 second;
		};

		uint32 resolution = 64;
		TArray<Line> lines;
		lines.Reserve(resolution * 3 + 3);

		Vec3 ax(1.f, 0.f, 0.f),
			ay(0.f, 1.f, 0.f),
			az(0.f, 0.f, 1.f);

		for (uint32 i = 0; i < resolution + 1; ++i)
		{
			float angle = TAU / resolution;
			float start = angle * i;

			float s1 = Math::Sin(start),
				s2 = Math::Sin(start + angle),
				c1 = Math::Cos(start),
				c2 = Math::Cos(start + angle);

			Line x, y, z;
			x = Line::Make(s1, s2, c1, c2, ay, az);
			y = Line::Make(s1, s2, c1, c2, az, ax);
			z = Line::Make(s1, s2, c1, c2, ax, ay);

			lines.Add(x);
			lines.Add(y);
			lines.Add(z);
		}

		mesh.vertex_buffer.Create(1);
		mesh.vertex_buffer.BindBuffer(0, 0, 3, 0, 0);
		mesh.vertex_buffer.BufferData(0, lines.Ptr(), sizeof(Line) * lines.Size());
		mesh.draw_mode = GL_LINES;
		mesh.draw_count = resolution * 2 * 3 + 3;
		mesh.draw_offset = 0;
		mesh.use_elements = false;
	}
}

DebugDrawManager::DebugDrawManager()
{
	mat_cube = &gResourceManager->Load<MaterialResource>("Material/Debug/cube.json")->material;
	mat_sphere = &gResourceManager->Load<MaterialResource>("Material/Debug/sphere.json")->material;
	mat_line = &gResourceManager->Load<MaterialResource>("Material/Debug/line.json")->material;

	mesh_cube = &gResourceManager->Load<MeshResource>("Mesh/Debug/cube.fbx")->mesh;
	LoadCubeLines(mesh_cube_lines);

	mesh_sphere = &gResourceManager->Load<MeshResource>("Mesh/Debug/sphere.fbx")->mesh;
	LoadSphereLines(mesh_sphere_lines);

	// Create line (will be filled in later)
	mesh_line.vertex_buffer.Create(1);
	mesh_line.vertex_buffer.BindBuffer(0, 0, 3, 7, 0);
	mesh_line.vertex_buffer.BindBuffer(0, 1, 4, 7, 3);
	mesh_line.draw_mode = GL_LINES;
	mesh_line.draw_offset = 0;
	mesh_line.use_elements = false;
}

void DebugDrawManager::AddCube(const Mat4& mat, const Color& color)
{
	Cube& cube = cubes.AddUninitialized();
	cube.mat = mat;
	cube.color = color;
}

void DebugDrawManager::AddSphere(const Vec3& origin, float radius, const Color& color)
{
	Sphere& sphere = spheres.AddUninitialized();
	sphere.origin = origin;
	sphere.radius = radius;
	sphere.color = color;
}

void DebugDrawManager::AddLine(const Vec3& start, const Vec3& end, const Color& color)
{
	Line& line = lines.AddUninitialized();
	line.vertices[0].position = start;
	line.vertices[0].color = color;
	line.vertices[1].position = end;
	line.vertices[1].color = color;
}

void DebugDrawManager::DrawAndClear(const Mat4& camera)
{
	glDepthFunc(GL_LEQUAL);

	// -- CUBES --
	{
		glUseProgram(mat_cube->program);
		mat_cube->Set("u_Camera", camera);

		for (uint32 i = 0; i < cubes.Size(); ++i)
		{
			mat_cube->Set("u_Model", cubes[i].mat);
			mat_cube->Set("u_Color", cubes[i].color * Color(0.6f, 0.6f, 0.6f, 1.f));
			mesh_cube->Draw();

			glDepthFunc(GL_LEQUAL);
			mat_cube->Set("u_Color", cubes[i].color);
			mesh_cube_lines.Draw();
			glDepthFunc(GL_LESS);
		}
	}

	// -- SPHERES --
	{
		glUseProgram(mat_sphere->program);
		mat_sphere->Set("u_Camera", camera);

		for (uint32 i = 0; i < spheres.Size(); ++i)
		{
			mat_sphere->Set("u_Origin", spheres[i].origin);
			mat_sphere->Set("u_Radius", spheres[i].radius);
			mat_sphere->Set("u_Color", spheres[i].color * Color(0.6f, 0.6f, 0.6f, 1.f));
			mesh_sphere->Draw();

			mat_sphere->Set("u_Color", spheres[i].color);
			mesh_sphere_lines.Draw();
		}
	}

	// -- LINES --
	{
		glUseProgram(mat_line->program);
		mat_line->Set("u_Camera", camera);
		mesh_line.vertex_buffer.BufferData(0, lines.Ptr(), sizeof(Line) * lines.Size());
		mesh_line.draw_count = lines.Size() * 2.f;
		mesh_line.Draw();
	}

	cubes.ClearNoDestruct();
	spheres.ClearNoDestruct();
	lines.ClearNoDestruct();

	glDepthFunc(GL_LESS);
}

