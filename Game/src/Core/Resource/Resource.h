#pragma once
#include <time.h>

class Resource
{
	friend class ResourceManager;

public:
	virtual bool LoadInternal(const char* path) = 0;
	virtual void UnloadInternal() = 0;
	void AddDependency(Resource* other);
	void ClearDependencies();
	const String& GetPath() const { return path; }

private:
	String path;
	time_t lastModifiedTime;

	Array<Resource*> dependecies;
	Array<Resource*> dependent;
};
