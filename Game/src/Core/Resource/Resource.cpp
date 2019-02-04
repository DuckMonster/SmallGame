
#include "Resource.h"
#include "ResourceManager.h"

void Resource::AddDependency(Resource* other)
{
	Assert(!dependecies.Contains(other));
	Assert(!other->dependent.Contains(this));

	dependecies.AddUnique(other);
	other->dependent.AddUnique(this);

	Debug_Log("%s --> %s", *path, *other->path);
}

void Resource::ClearDependencies()
{
	for (uint32 i = 0; i < dependecies.Size(); ++i)
	{
		dependecies[i]->dependent.Remove(this);
	}

	dependecies.Clear();
}