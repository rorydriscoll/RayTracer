#include <Core/Allocator.h>
#include <Core/Memory.h>

Allocator::Allocator(void* memory, size_t capacity) :
	memory(memory), capacity(capacity)
{
	Mark(memory, capacity, Memory::Pattern::Free);
}

Allocator::Allocator(Allocator& parent, size_t capacity) :
	parent(&parent), memory(parent.Allocate(capacity, 16)), capacity(capacity)
{
	Mark(memory, capacity, Memory::Pattern::Free);
}

Allocator::Allocator(Allocator&& other) noexcept :
	parent(other.parent), memory(other.memory), allocated(other.allocated), capacity(other.capacity)
{
	other.parent = nullptr;
	other.memory = nullptr;
	other.allocated = 0;
	other.capacity = 0;
}

Allocator& Allocator::operator=(Allocator&& other) noexcept
{
	if (parent)
		parent->Deallocate(memory);

	parent = other.parent;
	memory = other.memory;
	allocated = other.allocated;
	capacity = other.capacity;
	
	other.parent = nullptr;
	other.memory = nullptr;
	other.allocated = 0;
	other.capacity = 0;

	return *this;
}

Allocator::~Allocator()
{
	if (parent)
		parent->Deallocate(memory);
}
