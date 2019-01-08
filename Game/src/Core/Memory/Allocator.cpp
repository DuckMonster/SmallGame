
#include "Allocator.h"
#include "TempStack.h"

void* HeapAllocator::Malloc(uint32 size)
{
	return malloc(size);
}

void HeapAllocator::Free(void* ptr)
{
	free(ptr);
}

void* TemporaryAllocator::Malloc(uint32 size)
{
	return gTempStack->Malloc(size);
}
