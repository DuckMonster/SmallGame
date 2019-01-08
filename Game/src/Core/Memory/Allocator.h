#pragma once
class HeapAllocator
{
public:
	/* RAW ALLOCATIONS (pure data) */
	// Allocate size bytes of raw memory on the heap
	static void* Malloc(uint32 size);

	// Allocate raw memory of type T (does NOT call constructor)
	template<typename T>
	static T* Malloc() { return (T*)Malloc(sizeof(T)); }

	// Free a raw pointer
	static void Free(void* ptr);
};

class TemporaryAllocator
{
public:
	/* RAW ALLOCATIONS (pure data) */
	// Allocate size bytes of raw memory in the temporary storage
	static void* Malloc(uint32 size);

	// Allocate raw memory of type T (does NOT call constructor)
	template<typename T>
	static T* Malloc() { return (T*)Malloc(sizeof(T)); }

	// Free a raw pointer
	static void Free(void* ptr)
	{
		// Do nothing
	}
};