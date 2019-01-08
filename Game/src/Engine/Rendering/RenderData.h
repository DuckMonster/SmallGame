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

	uint32 passFlags = RP_Geometry | RP_Shadow;
	Mat4 matrix;
};

struct RenderCamera
{
	Mat4 matrix;
};

struct RenderDirectionalLight
{
	Mat4 matrix;
};

struct RenderScene
{
	RenderObject objects[MAX_OBJECTS];
	uint32 numObjects = 0;

	RenderCamera camera;
};