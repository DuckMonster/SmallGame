
#include "ResourceManager.h"
#include "Resource.h"

int ResourceScope::NumLoading = 0;
ResourceManager* gResourceManager = nullptr;

ResourceManager::ResourceManager(const char* root) : rootPath(Path::Clean(root))
{

}

ResourceManager::~ResourceManager()
{
	for(uint32 i=0; i<loadedResources.Size(); ++i)
		loadedResources[i]->UnloadInternal();
}

void ResourceManager::UpdateHotReloading()
{
	TArray<Resource*> resourceCopy = loadedResources;
	for(Resource* resource : resourceCopy)
	{
		if (HasNewerFile(resource))
		{
			Reload(resource);
		}
	}
}

bool ResourceManager::HasNewerFile(Resource* res)
{
	const char* path = *res->path;
	time_t modified = File::GetModifiedTime(path);

	return modified > res->lastModifiedTime;
}

void ResourceManager::Reload(Resource* res)
{
	const char* path = *res->path;
	time_t modified = File::GetModifiedTime(path);

	ResourceScope scope(path, true);

	res->UnloadInternal();
	res->LoadInternal(path);
	res->lastModifiedTime = modified;

	// Reload all dependent resources
	for(Resource* dependentResource : res->dependent)
	{
		Reload(dependentResource);
	}
}
