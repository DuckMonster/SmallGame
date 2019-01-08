
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

		mesh.vertexBuffer.Create(1);
		mesh.vertexBuffer.BindBuffer(0, 0, 3, 0, 0);
		mesh.vertexBuffer.BufferData(0, lines, sizeof(lines));
		mesh.drawMode = GL_LINES;
		mesh.drawCount = 2 * 12;
		mesh.useElements = false;
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

		mesh.vertexBuffer.Create(1);
		mesh.vertexBuffer.BindBuffer(0, 0, 3, 0, 0);
		mesh.vertexBuffer.BufferData(0, lines.Ptr(), sizeof(Line) * lines.Size());
		mesh.drawMode = GL_LINES;
		mesh.drawCount = resolution * 2 * 3 + 3;
		mesh.drawOffset = 0;
		mesh.useElements = false;
	}
}

DebugDrawManager::DebugDrawManager()
{
	cubeMaterial = &gResourceManager->Load<MaterialResource>("Material/Debug/cube.json")->material;
	sphereMaterial = &gResourceManager->Load<MaterialResource>("Material/Debug/sphere.json")->material;
	lineMaterial = &gResourceManager->Load<MaterialResource>("Material/Debug/line.json")->material;

	cubeMesh = &gResourceManager->Load<MeshResource>("Mesh/Debug/cube.fbx")->mesh;
	LoadCubeLines(cubeLineMesh);

	sphereMesh = &gResourceManager->Load<MeshResource>("Mesh/Debug/sphere.fbx")->mesh;
	LoadSphereLines(sphereLineMesh);

	// Create line (will be filled in later)
	lineMesh.vertexBuffer.Create(1);
	lineMesh.vertexBuffer.BindBuffer(0, 0, 3, 6, 0);
	lineMesh.vertexBuffer.BindBuffer(0, 1, 3, 6, 3);
	lineMesh.drawMode = GL_LINES;
	lineMesh.drawOffset = 0;
	lineMesh.useElements = false;
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
	// -- CUBES --
	{
		glUseProgram(cubeMaterial->program);
		cubeMaterial->Set("u_Camera", camera);

		for (uint32 i = 0; i < cubes.Size(); ++i)
		{
			cubeMaterial->Set("u_Model", cubes[i].mat);
			cubeMaterial->Set("u_Color", cubes[i].color * Color(0.6f, 0.6f, 0.6f, 1.f));
			cubeMesh->Draw();

			glDepthFunc(GL_LEQUAL);
			cubeMaterial->Set("u_Color", cubes[i].color);
			cubeLineMesh.Draw();
			glDepthFunc(GL_LESS);
		}
	}

	// -- SPHERES --
	{
		glUseProgram(sphereMaterial->program);
		sphereMaterial->Set("u_Camera", camera);

		for (uint32 i = 0; i < spheres.Size(); ++i)
		{
			sphereMaterial->Set("u_Origin", spheres[i].origin);
			sphereMaterial->Set("u_Radius", spheres[i].radius);
			sphereMaterial->Set("u_Color", spheres[i].color * Color(0.6f, 0.6f, 0.6f, 1.f));
			sphereMesh->Draw();

			glDepthFunc(GL_LEQUAL);
			sphereMaterial->Set("u_Color", spheres[i].color);
			sphereLineMesh.Draw();
			glDepthFunc(GL_LESS);
		}
	}

	// -- LINES --
	{
		lineMesh.vertexBuffer.BufferData(0, lines.Ptr(), sizeof(Line) * lines.Size());
		lineMesh.Draw();
	}

	cubes.ClearNoDestruct();
	spheres.ClearNoDestruct();
	lines.ClearNoDestruct();
}

