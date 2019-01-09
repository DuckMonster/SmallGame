#pragma once
#include "Core/OS/Path.h"
#include "Core/OS/File.h"
#include "Core/Standard/Map.h"

/* Helper class to print out which resources are loading, and their dependencies */
class ResourceScope
{
public:
	static int num_loading;

	ResourceScope(const char* path, bool hot_reload) : path(path)
	{
		for (int i = 0; i < num_loading; ++i)
			tab_string[i] = '\t';
		tab_string[num_loading] = '\0';

		Debug_Log("%s== %s resource '%s' ==", tab_string, hot_reload ? "Hot-reloading" : "Loading", path);
		num_loading++;
	}
	~ResourceScope()
	{
		Debug_Log("%s== Resource '%s' loaded ==", tab_string, path);
		num_loading--;
	}

private:
	const char* path;
	char tab_string[10];
};

class Resource;

/* Main manager for loading and keeping track of resources */
class ResourceManager
{
public:
	ResourceManager(const char* root);
	~ResourceManager();

	// Loads a reads a resource of type from path (path is relative from resource root)
	template<typename TRes>
	TRes* Load(const char* path);

	// Checks if any resources have updated on disk and needs to be reloaded
	void UpdateHotReloading();

	Map<String, Resource*> resource_map;
	Array<Resource*> loaded_resources;

private:
	bool HasNewerFile(Resource* res);
	void Reload(Resource* res);

	String root_path;
};

template<typename TRes>
TRes* ResourceManager::Load(const char* path)
{
	TString full_path = Path::Join(*root_path, path);

	// Find if the resource is already loaded
	{
		Resource** loaded_resource = resource_map.Find(full_path);
		if (loaded_resource != nullptr)
		{
			return (TRes*)(*loaded_resource);
		}
	}

	// Does the file exist?
	{
		FILE* res_file = fopen(*full_path, "r");
		if (res_file == NULL)
		{
			Fatal("Failed to open resource '%s'", path);
			return nullptr;
		}

		fclose(res_file);
	}

	// Load it!
	ResourceScope scope(path, false);
	TRes* res = new TRes();

	resource_map[full_path] = res;
	loaded_resources.Add(res);

	// We want to create the resource even if it doesn't exist,
	// since we want to hot-reload it once it does!
	res->path = full_path;
	res->LoadInternal(*full_path);
	res->last_modified_time = File::GetModifiedTime(*full_path);

	return res;
}

extern ResourceManager* gResourceManager;
