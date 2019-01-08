#pragma once
#include "Core/OS/Path.h"
#include "Core/OS/File.h"
#include "Core/Standard/Map.h"

/* Helper class to print out which resources are loading, and their dependencies */
class ResourceScope
{
public:
	static int NumLoading;

	ResourceScope(const char* path, bool hotReload) : path(path)
	{
		for (int i = 0; i < NumLoading; ++i)
			tabString[i] = '\t';
		tabString[NumLoading] = '\0';

		Debug_Log("%s== %s resource '%s' ==", tabString, hotReload ? "Hot-reloading" : "Loading", path);
		NumLoading++;
	}
	~ResourceScope()
	{
		Debug_Log("%s== Resource '%s' loaded ==", tabString, path);
		NumLoading--;
	}

private:
	const char* path;
	char tabString[10];
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

	Map<String, Resource*> resourceMap;
	Array<Resource*> loadedResources;

private:
	bool HasNewerFile(Resource* res);
	void Reload(Resource* res);

	String rootPath;
};

template<typename TRes>
TRes* ResourceManager::Load(const char* path)
{
	TString fullPath = Path::Join(*rootPath, path);

	// Find if the resource is already loaded
	{
		Resource** loadedResource = resourceMap.Find(fullPath);
		if (loadedResource != nullptr)
		{
			return (TRes*)(*loadedResource);
		}
	}

	// Does the file exist?
	{
		FILE* resFile = fopen(*fullPath, "r");
		if (resFile == NULL)
		{
			Fatal("Failed to open resource '%s'", path);
			return nullptr;
		}

		fclose(resFile);
	}

	// Load it!
	ResourceScope scope(path, false);
	TRes* res = new TRes();

	resourceMap[fullPath] = res;
	loadedResources.Add(res);

	// We want to create the resource even if it doesn't exist,
	// since we want to hot-reload it once it does!
	res->path = fullPath;
	res->LoadInternal(*fullPath);
	res->lastModifiedTime = File::GetModifiedTime(*fullPath);

	return res;
}

extern ResourceManager* gResourceManager;
