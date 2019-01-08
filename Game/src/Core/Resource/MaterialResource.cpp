
#include "MaterialResource.h"
#include "Core/Serialize/Json.h"
#include "ResourceManager.h"

bool MaterialResource::LoadInternal(const char* path)
{
	JsonDocument document;
	document.LoadFile(path);

	JsonValue object = document.GetRootObject();

	const char* parentPath = nullptr;
	const char* vertPath = nullptr;
	const char* fragPath = nullptr;
	const char* texturePath = nullptr;
	object.Serialize("parent", parentPath);
	object.Serialize("vertex", vertPath);
	object.Serialize("fragment", fragPath);
	object.Serialize("texture", texturePath);

	// Load parent material
	if (parentPath != nullptr)
	{
		parent = gResourceManager->Load<MaterialResource>(parentPath);
		if (parent == nullptr)
		{
			Error("Failed to load parent '%s' when loading material '%s'", parentPath, path);
		}
		else
		{
			AddDependency(parent);
		}
	}

	// Load vertex shader
	if (vertPath != nullptr)
	{
		vertexResource = gResourceManager->Load<ShaderResource>(vertPath);
	}
	else if (parent != nullptr)
	{
		vertexResource = parent->vertexResource;
	}

	// Load fragment shader
	if (fragPath != nullptr)
	{
		fragmentResource = gResourceManager->Load<ShaderResource>(fragPath);
	}
	else if (parent != nullptr)
	{
		fragmentResource = parent->fragmentResource;
	}

	// Check to make sure we have all shaders specified
	if (vertexResource == nullptr)
	{
		Error("No vertex shader specified for material '%s'", path);
		vertexResource = fragmentResource = nullptr;
		parent = nullptr;
		return true;
	}
	if (fragmentResource == nullptr)
	{
		Error("No fragment shader specified for material '%s'", path);
		vertexResource = fragmentResource = nullptr;
		parent = nullptr;
		return true;
	}

	AddDependency(vertexResource);
	AddDependency(fragmentResource);

	// Load texture
	if (texturePath != nullptr)
	{
		textureResource = gResourceManager->Load<TextureResource>(texturePath);
		if (textureResource == nullptr)
		{
			Error("Failed to load texture '%s' when loading material '%s'", texturePath, path);
		}
	}
	else if (parent != nullptr)
	{
		textureResource = parent->textureResource;
	}

	// If a texture was aquired somehow, depend on it!
	if (textureResource != nullptr)
		AddDependency(textureResource);

	if (vertexResource->isValid && fragmentResource->isValid)
	{
		// Create and link program, only if both shaders loaded properly
		material.program = glCreateProgram();
		glAttachShader(material.program, vertexResource->handle);
		glAttachShader(material.program, fragmentResource->handle);
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

		if (textureResource != nullptr)
		{
			material.texture = textureResource->texture;
		}
		else
		{
			material.texture = Texture();
		}

		isValid = true;
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
	if (isValid)
	{
		UniformCache::InvalidateCacheFor(material.program);
		glDeleteProgram(material.program);
	}

	ClearDependencies();

	vertexResource = fragmentResource = nullptr;
	textureResource = nullptr;
	parent = nullptr;

	isValid = false;
}
