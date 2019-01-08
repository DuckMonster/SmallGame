
#include "TempStack.h"
#include "Core/Math/Math.h"

TempStack* gTempStack = nullptr;

void* TempStack::Malloc(uint32 size)
{
	// Resize needed
	if (offset + size > dataSize)
	{
		uint32 newSize = Math::FindClosestLargerPo2(offset + size);
		Resize(newSize);
	}

	void* alloc = data + offset;
	offset += size;

	return alloc;
}

void TempStack::Resize(uint32 newSize)
{
	// Save current memory as stale, since other allocations may still be in use
	if (data != nullptr)
	{
		staleMemory.Add(data);
	}

	data = (uint8*)malloc(newSize);
	dataSize = newSize;
	offset = 0;

	Debug_Log("TempBuffer size: %d", dataSize);
}

void TempStack::Reset()
{
	prevOffset = offset;
	offset = 0;

	// Finally free all the stale memory saved from resizing the buffer */
	for(uint32 i = 0; i<staleMemory.Size(); ++i)
	{
		free(staleMemory[i]);
	}
	staleMemory.Clear();
}
