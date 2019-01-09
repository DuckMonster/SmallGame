
#include "MaterialResource.h"
#include "Core/Serialize/Json.h"
#include "ResourceManager.h"

bool MaterialResource::LoadInternal(const char* path)
{
	JsonDocument document;
	document.LoadFile(path);

	JsonValue object = document.GetRootObject();

	const char* parent_path = nullptr;
	const char* vert_path = nullptr;
	const char* frag_path = nullptr;
	const char* texture_path = nullptr;
	object.Serialize("parent", parent_path);
	object.Serialize("vertex", vert_path);
	object.Serialize("fragment", frag_path);
	object.Serialize("texture", texture_path);

	// Load parent material
	if (parent_path != nullptr)
	{
		parent = gResourceManager->Load<MaterialResource>(parent_path);
		if (parent == nullptr)
		{
			Error("Failed to load parent '%s' when loading material '%s'", parent_path, path);
		}
		else
		{
			AddDependency(parent);
		}
	}

	// Load vertex shader
	if (vert_path != nullptr)
	{
		vertex_resource = gResourceManager->Load<ShaderResource>(vert_path);
	}
	else if (parent != nullptr)
	{
		vertex_resource = parent->vertex_resource;
	}

	// Load fragment shader
	if (frag_path != nullptr)
	{
		fragment_resource = gResourceManager->Load<ShaderResource>(frag_path);
	}
	else if (parent != nullptr)
	{
		fragment_resource = parent->fragment_resource;
	}

	// Check to make sure we have all shaders specified
	if (vertex_resource == nullptr)
	{
		Error("No vertex shader specified for material '%s'", path);
		vertex_resource = fragment_resource = nullptr;
		parent = nullptr;
		return true;
	}
	if (fragment_resource == nullptr)
	{
		Error("No fragment shader specified for material '%s'", path);
		vertex_resource = fragment_resource = nullptr;
		parent = nullptr;
		return true;
	}

	AddDependency(vertex_resource);
	AddDependency(fragment_resource);

	// Load texture
	if (texture_path != nullptr)
	{
		texture_resource = gResourceManager->Load<TextureResource>(texture_path);
		if (texture_resource == nullptr)
		{
			Error("Failed to load texture '%s' when loading material '%s'", texture_path, path);
		}
	}
	else if (parent != nullptr)
	{
		texture_resource = parent->texture_resource;
	}

	// If a texture was aquired somehow, depend on it!
	if (texture_resource != nullptr)
		AddDependency(texture_resource);

	if (vertex_resource->is_valid && fragment_resource->is_valid)
	{
		// Create and link program, only if both shaders loaded properly
		material.program = glCreateProgram();
		glAttachShader(material.program, vertex_resource->handle);
		glAttachShader(material.program, fragment_resource->handle);
		glLinkProgram(material.program);

		// Get link status
		GLint success;
		glGetProgramiv(material.program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			// Uh oh
			char* buffer = talloc(1024);
			glGetProgramInfoLog(material.program, 1024, nullptr, buffer);
			Debug_Log("Program link failed\n%s", buffer);
		}

		if (texture_resource != nullptr)
		{
			material.texture = texture_resource->texture;
		}
		else
		{
			material.texture = Texture();
		}

		is_valid = true;
	}

	// Uh oh, invalid shaders, bad news
	else
	{
		Debug_Log("One or more shaders were invalid when loading material '%s', skipping...", path);
	}

	return true;
}

void MaterialResource::UnloadInternal()
{
	// Invalidate uniform cache, since recompilation most probably will make new uniform indicies
	if (is_valid)
	{
		UniformCache::InvalidateCacheFor(material.program);
		glDeleteProgram(material.program);
	}

	ClearDependencies();

	vertex_resource = fragment_resource = nullptr;
	texture_resource = nullptr;
	parent = nullptr;

	is_valid = false;
}
