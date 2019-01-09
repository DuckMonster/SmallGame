
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
	for (uint32 mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index)
	{
		aiMesh* scene_mesh = scene->mMeshes[mesh_index];

		uint32 mesh_verts = scene_mesh->mNumVertices;
		uint32 mesh_indices = scene_mesh->mNumFaces * 3;

		Vec3* position_data = new Vec3[mesh_verts];
		Vec3* normal_data = new Vec3[mesh_verts];
		Vec3* uv_data = new Vec3[mesh_verts];  // The uv data has 3 components, for cubemapped meshes?
		uint32* index_data = new uint32[mesh_indices];

		defer
		{
			delete[] position_data;
			delete[] normal_data;
			delete[] uv_data;
			delete[] index_data;
		};

		memcpy(position_data, scene_mesh->mVertices, sizeof(Vec3) * mesh_verts);

		if (scene_mesh->HasNormals())
			memcpy(normal_data, scene_mesh->mNormals, sizeof(Vec3) * mesh_verts);

		if (scene_mesh->HasTextureCoords(0))
			memcpy(uv_data, scene_mesh->mTextureCoords[0], sizeof(Vec3) * mesh_verts);

		// Fetch indexes
		for (uint32 face_index = 0; face_index < scene_mesh->mNumFaces; ++face_index)
		{
			const aiFace& face = scene_mesh->mFaces[face_index];
			Assert(face.mNumIndices == 3);  // Right now we only support triangles

			memcpy(index_data + 3 * face_index, face.mIndices, sizeof(uint32) * 3);
		}

		VertexBuffer& buffer = mesh.vertex_buffer;
		buffer.Create(4);

		// Upload and bind all the buffers
		{
			buffer.BufferData(0, position_data, sizeof(Vec3) * mesh_verts);
			buffer.BindBuffer(0, 0, 3, 0, 0);

			buffer.BufferData(1, normal_data, sizeof(Vec3) * mesh_verts);
			buffer.BindBuffer(1, 1, 3, 0, 0);

			buffer.BufferData(2, uv_data, sizeof(Vec3) * mesh_verts);
			buffer.BindBuffer(2, 2, 2, 3, 0);

			buffer.BufferElementData(3, index_data, sizeof(GLuint) * mesh_indices);
		}

		mesh.draw_count = mesh_indices;
		mesh.draw_offset = 0;
		mesh.draw_mode = GL_TRIANGLES;
		mesh.use_elements = true;

		break;
	}

	return true;
}

void MeshResource::UnloadInternal()
{
	mesh.vertex_buffer.Destroy();
}
