#include <Core/HeapAllocator.h>
#include <Core/Log.h>
#include <Core/Memory.h>
#include <Core/Pointer.h>
#include <Core/Assert.h>
#include <Core/Constants.h>

namespace
{
	struct Header
	{
		// The unaligned memory
		//
		void* memory = nullptr;

		// Unaligned memory size
		//
		size_t size = 0;
	};

	bool ValidateFreeList(const HeapAllocator::FreeBlock* head)
	{
		const HeapAllocator::FreeBlock* prev_block = nullptr;

		for (const HeapAllocator::FreeBlock* block = head; block; block = block->next)
		{
			if (prev_block)
			{
				if (Pointer::Offset(prev_block, prev_block->size) > block)
					return false;
			}

			const HeapAllocator::FreeBlock* next_block = block->next;

			if (next_block)
			{
				if (Pointer::Offset(block, block->size) > next_block)
					return false;
			}
		}

		return true;
	}
}

HeapAllocator::HeapAllocator(void* memory, size_t size) :
	Allocator(memory, size), head(static_cast<FreeBlock*>(memory))
{
	head->size = size;
	head->next = nullptr;
}

HeapAllocator::HeapAllocator(Allocator& parent, size_t size) :
	Allocator(parent, size), head(static_cast<FreeBlock*>(memory))
{
	ASSERT(capacity >= sizeof(FreeBlock), "Heap too small!");

	head->size = size;
	head->next = nullptr;

	LOG_INFO("Create heap (%iMiB)", capacity / 1_MiB);
}

HeapAllocator::~HeapAllocator()
{
	ASSERT(IsEmpty(), "Heap has outstanding allocations (%llu bytes allocated)", capacity - CalculateFreeSpace());
}

void* HeapAllocator::Allocate(size_t size, size_t alignment)
{
	if (size == 0)
		return nullptr;

	for (FreeBlock* block = head, *prev = nullptr; block; prev = block, block = block->next)
	{
		// Calculate the adjustment required to maintain alignment and padding for this block

		const size_t adjustment = Pointer::CalculateAdjustment(block, sizeof(Header), alignment);

		// Skip if the block is too small to hold the complete allocation

		size_t total = size + adjustment;

		if (block->size < total)
			continue;

		// Calculate how much space will be left unused in the block

		const size_t remainder = block->size - total;

		FreeBlock* next;

		if (remainder < sizeof(Header))
		{
			// If the remainder is too small to hold an allocation then absorb it and remove
			// the entire block from the free list.

			total += remainder;
			next = block->next;
		}
		else
		{
			// There is usable space left in the current block, so create a new block
			// header and insert it into the free list.

			next = static_cast<FreeBlock*>(Pointer::Offset(block, total));

			next->size = remainder;
			next->next = block->next;
		}

		// Update the previous block to point to the new next block

		if (prev)
			prev->next = next;
		else
			head = next;

		// Calculate the address of the aligned allocation and fill with marker patterns

		void* unaligned = block;
		void* aligned = Pointer::Offset(unaligned, adjustment);

		Mark(unaligned, adjustment, Memory::Pattern::Guard);
		Mark(aligned, size, Memory::Pattern::Allocated);
		Mark(Pointer::Offset(aligned, size), total - size - adjustment, Memory::Pattern::Guard);

		// Fill out the allocation details

		Header* header = static_cast<Header*>(aligned) - 1;

		header->memory = unaligned;
		header->size = total;

		// Save off stats
		
		allocated += total;

		// Return the aligned memory

		return aligned;
	}

	CRITICAL(false, "Heap (%.2f MiB) ran out of memory trying to allocate %.2f MiB", capacity / float(1_MiB), size / float(1_MiB));

	return nullptr;
}

void HeapAllocator::Deallocate(void* ptr)
{
	// Exit on null free

	if (!ptr)
		return;

	// Take a copy of the allocation header since the header will be overwritten

	const Header header = *(static_cast<const Header*>(ptr) - 1);

	ASSERT(header.memory != nullptr, "Invalid allocation memory");

	// Registers statistics

	allocated -= header.size;

	// Mark the memory as free

	Mark(header.memory, header.size, Memory::Pattern::Free);

	// Prepare a new head list block

	FreeBlock* block = static_cast<FreeBlock*>(header.memory);

	block->size = header.size;

	// Find the correct place in the free list to insert the new block

	FreeBlock* prev_block = nullptr;
	FreeBlock* next_block = head;

	while (next_block)
	{
		if (next_block > block)
			break;

		prev_block = next_block;
		next_block = next_block->next;
	}

	// Attempt to coalesce with the next block. Do this before coalescing with the previous block

	if (next_block && Pointer::Offset(block, block->size) == next_block)
	{
		// Add to the new block size

		block->size += next_block->size;

		// Ignore the next block

		next_block = next_block->next;

		// We have effectively removed the next block from the free list, so for the sake
		// of keeping the free list valid, update the previous block's next pointer. This is
		// required in case the current block also gets coalesced into the previous one.

		if (prev_block)
			prev_block->next = next_block;
	}

	// Attempt to coalesce with the previous block

	if (prev_block && Pointer::Offset(prev_block, prev_block->size) == block)
	{
		// Add to the previous block size

		prev_block->size += block->size;

		// Exit now since we don't need to add a new free list entry

		return;
	}

	// Hook the new block into the free list

	block->next = next_block;

	if (prev_block)
		prev_block->next = block;
	else
		head = block;

	ASSERT(head->size <= capacity, "FOO");
}

bool HeapAllocator::IsEmpty() const
{
	return !head || head->size == capacity;
}

size_t HeapAllocator::CalculateFreeSpace() const
{
	size_t free = 0;

	for (FreeBlock* block = head; block; block = block->next)
		free += block->size;

	return free;
}
