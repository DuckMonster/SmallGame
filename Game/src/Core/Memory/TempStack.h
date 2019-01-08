#pragma once
#define talloc(size) ((char*)gTempStack->Malloc(size))

class TempStack
{
public:
	void* Malloc(uint32 size);
	template<typename T>
	T* Malloc()
	{
		return (T*)Malloc(sizeof(T));
	}

	template<typename T>
	T* Malloc(uint32 count)
	{
		return (T*)Malloc(sizeof(T) * count);
	}

	void Reset();
	void Resize(uint32 newSize);

	uint8* data = nullptr;
	Array<uint8*> staleMemory;
	uint32 offset = 0;
	uint32 dataSize = 0;

	// Data used for profiling
	uint32 prevOffset;
};

extern TempStack* gTempStack;
