#pragma once
class File
{
public:
	// Reads all contents of a file into a pre-allocated buffer
	static bool ReadAllStatic(const char* path, char* buffer, uint32 size);

	// Reads all contents of a file, and will allocate a buffer to fit the file being read
	static bool ReadAllDynamic(const char* path, char*& buffer);

	// Reads all contents of a file, and will allocate a buffer in temporary storage
	static bool ReadAllTemporary(const char* path, char*& buffer);

	// Gets the time a file what last modified
	static time_t GetModifiedTime(const char* path);
};
