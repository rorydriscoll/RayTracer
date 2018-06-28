#include <Core/Memory.h>
#include <Core/Constants.h>
#include <Core/HeapAllocator.h>
#include <cstring>

Allocator& Memory::TempAllocator()
{
	static char memory[100_MiB] = {};
	static HeapAllocator allocator(memory, sizeof(memory));

	return allocator;
}

void Memory::Copy(void* dest, const void* source, size_t size)
{
	memcpy(dest, source, size);
}

void Memory::Mark(void* memory, size_t size, Pattern pattern)
{
	memset(memory, pattern, size);
}
