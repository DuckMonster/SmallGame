
#include "ResourceManager.h"
#include "Resource.h"

int ResourceScope::num_loading = 0;
ResourceManager* gResourceManager = nullptr;

ResourceManager::ResourceManager(const char* root) : root_path(Path::Clean(root))
{

}

ResourceManager::~ResourceManager()
{
	for(uint32 i=0; i<loaded_resources.Size(); ++i)
		loaded_resources[i]->UnloadInternal();
}

void ResourceManager::UpdateHotReloading()
{
	TArray<Resource*> resource_copy = loaded_resources;
	for(Resource* resource : resource_copy)
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

	return modified > res->last_modified_time;
}

void ResourceManager::Reload(Resource* res)
{
	const char* path = *res->path;
	time_t modified = File::GetModifiedTime(path);

	ResourceScope scope(path, true);

	res->UnloadInternal();
	res->LoadInternal(path);
	res->last_modified_time = modified;

	// Reload all dependent resources
	for(Resource* dependent_resource : res->dependent)
	{
		Reload(dependent_resource);
	}
}
