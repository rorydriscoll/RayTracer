#pragma once

#include <Core/Types.h>

struct Allocator;

namespace Memory
{
	enum Pattern : uint8_t
	{
		Guard = 0xfd,
		Allocated = 0xcd,
		Free = 0xdd
	};

	// Allocator for short-lived allocations
	//
	Allocator& TempAllocator();

	// Copy
	//
	void Copy(void* dest, const void* source, size_t size);

	// Mark the memory with the supplied pattern
	//
	void Mark(void* memory, size_t size, Pattern pattern);
}