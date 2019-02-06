
#include "TempStack.h"
#include "Core/Math/Math.h"

TempStack temp_stack;

void* TempStack::Malloc(uint32 size)
{
	// Resize needed
	if (offset + size > data_size)
	{
		uint32 new_size = Math::FindClosestLargerPo2(offset + size);
		Resize(new_size);
	}

	void* alloc = data + offset;
	offset += size;

	return alloc;
}

void TempStack::Resize(uint32 new_size)
{
	// Save current memory as stale, since other allocations may still be in use
	if (data != nullptr)
	{
		stale_memory.Add(data);
	}

	data = (uint8*)malloc(new_size);
	data_size = new_size;
	offset = 0;

	Debug_Log("TempBuffer size: %d", data_size);
}

void TempStack::Reset()
{
	prev_offset = offset;
	offset = 0;

	// Finally free all the stale memory saved from resizing the buffer */
	for(uint32 i = 0; i<stale_memory.Size(); ++i)
	{
		free(stale_memory[i]);
	}
	stale_memory.Clear();
}
