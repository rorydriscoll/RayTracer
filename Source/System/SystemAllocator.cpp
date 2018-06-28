#include <System/SystemAllocator.h>
#include <System/Windows.h>
#include <Core/Assert.h>

SystemAllocator::SystemAllocator(size_t size) : HeapAllocator(VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE | MEM_TOP_DOWN, PAGE_READWRITE), size)
{
}

SystemAllocator::~SystemAllocator()
{
	ASSERT(IsEmpty(), "Heap has outstanding allocations");

	VirtualFree(memory, 0, MEM_RELEASE);

	// The system memory may be overwritten with a memory pattern, so clear out the memory
	// pointer so that the heap destructor doesn't try to read bad data.

	memory = nullptr;
	head = nullptr;
}
