#pragma once
#include <vector>
#include "Core/Render/Material.h"
#include "Core/Render/Mesh.h"

#define MAX_OBJECTS 1024

enum RenderPass
{
	RP_Geometry	= 1 << 0,
	RP_Shadow	= 1 << 1
};

struct RenderObject
{
	Mesh mesh;
	Material material;

	uint32 pass_flag = RP_Geometry | RP_Shadow;
	Mat4 matrix;
};

struct RenderCamera
{
	Mat4 matrix;
};

struct RenderDirectionalLight
{
	Vec3 position;
	Vec3 direction;
};

struct RenderScene
{
	RenderObject objects[MAX_OBJECTS];
	uint32 object_num = 0;

	RenderCamera camera;
	RenderDirectionalLight light;
};