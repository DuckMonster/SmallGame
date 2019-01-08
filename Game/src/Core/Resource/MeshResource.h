#pragma once
#include "Core/Render/Mesh.h"
#include "Resource.h"

class MeshResource : public Resource
{
public:
	bool LoadInternal(const char* path) override;
	void UnloadInternal() override;

	Mesh mesh;
};
