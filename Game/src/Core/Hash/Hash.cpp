
#include "Hash.h"

namespace
{
	const uint32 Prime = 0x01000193;
	const uint32 Seed = 0x811C9DC5;
}

// Simple FNV hashing
uint32 Hash::Get(const void* data, uint32 length)
{
	uint32 hash = Seed;
	uint8* ptr = (uint8*)data;
	while (length--)
	{
		hash = (*ptr++ ^ hash) * Prime;
	}
	return hash;
}
