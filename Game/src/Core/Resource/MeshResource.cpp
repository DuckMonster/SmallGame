
#include "MeshResource.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "ResourceManager.h"

bool MeshResource::LoadInternal(const char* path)
{
	using namespace Assimp;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene)
	{
		Error("Failed to load mesh \"%s\": %s", path, importer.GetErrorString());
		return false;
	}

	/* **** LOAD MESHES **** */
	for (uint32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* sceneMesh = scene->mMeshes[meshIndex];

		uint32 meshVerts = sceneMesh->mNumVertices;
		uint32 meshIndices = sceneMesh->mNumFaces * 3;

		Vec3* positionData = new Vec3[meshVerts];
		Vec3* normalData = new Vec3[meshVerts];
		Vec3* uvData = new Vec3[meshVerts];  // The uv data has 3 components, for cubemapped meshes?
		uint32* indexData = new uint32[meshIndices];

		defer
		{
			delete[] positionData;
			delete[] normalData;
			delete[] uvData;
			delete[] indexData;
		};

		memcpy(positionData, sceneMesh->mVertices, sizeof(Vec3) * meshVerts);

		if (sceneMesh->HasNormals())
			memcpy(normalData, sceneMesh->mNormals, sizeof(Vec3) * meshVerts);

		if (sceneMesh->HasTextureCoords(0))
			memcpy(uvData, sceneMesh->mTextureCoords[0], sizeof(Vec3) * meshVerts);

		// Fetch indexes
		for (uint32 faceIndex = 0; faceIndex < sceneMesh->mNumFaces; ++faceIndex)
		{
			const aiFace& face = sceneMesh->mFaces[faceIndex];
			Assert(face.mNumIndices == 3);  // Right now we only support triangles

			memcpy(indexData + 3 * faceIndex, face.mIndices, sizeof(uint32) * 3);
		}

		VertexBuffer& buffer = mesh.vertexBuffer;
		buffer.Create(4);

		// Upload and bind all the buffers
		{
			buffer.BufferData(0, positionData, sizeof(Vec3) * meshVerts);
			buffer.BindBuffer(0, 0, 3, 0, 0);

			buffer.BufferData(1, normalData, sizeof(Vec3) * meshVerts);
			buffer.BindBuffer(1, 1, 3, 0, 0);

			buffer.BufferData(2, uvData, sizeof(Vec3) * meshVerts);
			buffer.BindBuffer(2, 2, 2, 3, 0);

			buffer.BufferElementData(3, indexData, sizeof(GLuint) * meshIndices);
		}

		mesh.drawCount = meshIndices;
		mesh.drawOffset = 0;
		mesh.drawMode = GL_TRIANGLES;
		mesh.useElements = true;

		break;
	}

	return true;
}

void MeshResource::UnloadInternal()
{
	mesh.vertexBuffer.Destroy();
}
