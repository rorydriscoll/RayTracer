#include <Core/StackAllocator.h>
#include <Core/Pointer.h>
#include <Core/Memory.h>

StackAllocator::StackAllocator(void* memory, size_t capacity) :
	Allocator(memory, capacity), head(memory)
{
	Mark(memory, capacity, Memory::Pattern::Free);
}

StackAllocator::StackAllocator(Allocator& parent, size_t capacity) : 
	Allocator(parent, capacity), head(memory)
{
}

void StackAllocator::Clear()
{
	head = memory;
	allocated = 0;
}

void* StackAllocator::Allocate(size_t size, size_t alignment)
{
	ASSERT(Pointer::Offset(Pointer::Align(head, alignment), size) <= Pointer::Offset(memory, capacity), "Allocator is out of memory");

	if (size == 0)
		return nullptr;

	void* allocation = Pointer::Align(head, alignment);

	Mark(head, alignment, Memory::Pattern::Guard);
	Mark(allocation, size, Memory::Pattern::Allocated);

	head = Pointer::Offset(allocation, size);
	allocated += size;

	return allocation;
}

void StackAllocator::Deallocate(void*)
{
}

size_t StackAllocator::BytesFree() const
{
	return capacity - allocated;
}

