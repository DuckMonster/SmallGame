
#include "MaterialResource.h"
#include "Core/Serialize/Json.h"
#include "ResourceManager.h"

namespace
{
	GLuint CreateAndCompileShader(const char* src, GLenum type)
	{
		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, &src, nullptr);
		glCompileShader(handle);

		return handle;
	}
}

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
		parent = resource_manager.Load<MaterialResource>(parent_path);
		if (parent == nullptr)
		{
			Error("Failed to load parent '%s' when loading material '%s'", parent_path, path);
		}
		else
		{
			AddDependency(parent);
			parameters = parent->parameters;
		}
	}

	// Load shader parameters
	Map<String, JsonValue> loaded_params;
	object["parameters"].SerializeChildren(loaded_params);
	for(const KeyValuePair<String, JsonValue>& pair : loaded_params)
	{
		JsonValue value = pair.value;
		TString str_value;

		// Arrays (or vectors)
		if (value.IsArray())
		{
			uint32 len = value.ArraySize();
			switch(len)
			{
				case 2:
				{
					Vec2 vec;
					value.Serialize(vec);

					str_value = TString::Printf("vec2(%f, %f)", vec.x, vec.y);
					break;
				}
				case 3:
				{
					Vec3 vec;
					value.Serialize(vec);

					str_value = TString::Printf("vec3(%f, %f, %f)", vec.x, vec.y, vec.z);
					break;
				}
				case 4:
				{
					Vec4 vec;
					value.Serialize(vec);

					str_value = TString::Printf("vec4(%f, %f, %f, %f)", vec.x, vec.y, vec.z, vec.w);
					break;
				}
			}
		}
		// Float
		else if (value.Is<float>())
		{
			float val;
			value.Serialize(val);

			str_value = TString::Printf("%f", val);
		}
		// Integer
		else if (value.Is<int>())
		{
			int val;
			value.Serialize(val);

			str_value = TString::Printf("%d", val);
		}
		// String
		else if (value.Is<const char*>())
		{
			value.Serialize(str_value);
		}
		else
		{
			Error("Invalid parameter type '%s' for material '%s'", *pair.key, path);
			continue;
		}

		parameters[pair.key] = str_value;
	}

	Debug_Log("Parameters for '%s':", path);
	for(const KeyValuePair<String, String>& pair : parameters)
	{
		Debug_Log("%s: %s", *pair.key, *pair.value);
	}

	// Load vertex shader
	if (vert_path != nullptr)
	{
		vertex_resource = resource_manager.Load<ShaderResource>(vert_path);

		if (vertex_resource != nullptr)
			AddDependency(vertex_resource);
	}
	else if (parent != nullptr)
	{
		vertex_resource = parent->vertex_resource;
	}

	// Load fragment shader
	if (frag_path != nullptr)
	{
		fragment_resource = resource_manager.Load<ShaderResource>(frag_path);

		if (fragment_resource != nullptr)
			AddDependency(fragment_resource);
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

	// Load texture
	if (texture_path != nullptr)
	{
		texture_resource = resource_manager.Load<TextureResource>(texture_path);
		if (texture_resource == nullptr)
			Error("Failed to load texture '%s' when loading material '%s'", texture_path, path);
		else
			AddDependency(texture_resource);
	}
	else if (parent != nullptr)
	{
		texture_resource = parent->texture_resource;
	}

	// Load shaders
	vertex_handle = vertex_resource->Compile(parameters);
	fragment_handle = fragment_resource->Compile(parameters);

	// Create and link program, only if both shaders loaded properly
	material.program = glCreateProgram();
	glAttachShader(material.program, vertex_handle);
	glAttachShader(material.program, fragment_handle);
	glLinkProgram(material.program);

	// Get link status
	GLint success;
	glGetProgramiv(material.program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Uh oh
		char* buffer = talloc(1024);
		glGetProgramInfoLog(material.program, 1024, nullptr, buffer);
		Error("Program link failed\n%s", buffer);
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
	return true;
}

void MaterialResource::UnloadInternal()
{
	// Invalidate uniform cache, since recompilation most probably will make new uniform indicies
	if (is_valid)
	{
		UniformCache::InvalidateCacheFor(material.program);
		glDeleteProgram(material.program);

		glDeleteShader(vertex_handle);
		glDeleteShader(fragment_handle);
	}

	ClearDependencies();

	vertex_resource = fragment_resource = nullptr;
	texture_resource = nullptr;
	parent = nullptr;

	parameters.Clear();

	is_valid = false;
}
