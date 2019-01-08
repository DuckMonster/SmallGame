
#include "File.h"

bool File::ReadAllStatic(const char* path, char* buffer, uint32 size)
{
	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		Error("Failed loading file '%s'", path);
		return false;
	}

	defer { fclose(file); };

	// Get length of file
	fseek(file, 0, SEEK_END);
	uint32 length = ftell(file);
	fseek(file, 0, SEEK_SET);

	Assert(length < size);

	// Read it all
	size_t bytesRead = fread(buffer, 1, Math::Max(length, size), file);
	buffer[bytesRead] = '\0';
	return true;
}

bool File::ReadAllDynamic(const char* path, char*& buffer)
{
	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		Error("Failed loading file '%s'", path);
		return false;
	}

	defer { fclose(file); };

	// Get length of file
	int result = fseek(file, 0, SEEK_END);
	long length = ftell(file);
	rewind(file);

	// Allocate buffer
	buffer = new char[length + 1];

	// Read it all
	size_t bytesRead = fread(buffer, 1, length, file);
	buffer[bytesRead] = '\0';
	return true;
}

bool File::ReadAllTemporary(const char* path, char*& buffer)
{
	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		Error("Failed loading file '%s'", path);
		return false;
	}

	defer { fclose(file); };

	// Get length of file
	int result = fseek(file, 0, SEEK_END);
	long length = ftell(file);
	rewind(file);

	// Allocate buffer
	buffer = talloc(length + 1);

	// Read it all
	size_t bytesRead = fread(buffer, 1, length, file);
	buffer[bytesRead] = '\0';
	return true;
}

time_t File::GetModifiedTime(const char* path)
{
	struct stat result;
	if (stat(path, &result) == 0)
	{
		return result.st_mtime;
	}

	return 0;
}
