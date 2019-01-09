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
	void Resize(uint32 new_size);

	uint8* data = nullptr;
	Array<uint8*> stale_memory;
	uint32 offset = 0;
	uint32 data_size = 0;

	// Data used for profiling
	uint32 prev_offset;
};

extern TempStack* gTempStack;
