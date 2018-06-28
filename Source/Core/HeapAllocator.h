#pragma once

#include <Core/Allocator.h>

struct HeapAllocator : Allocator
{
	struct FreeBlock
	{
		// Total size of the block in byts
		//
		size_t size = 0;

		// Next block in the free list
		//
		FreeBlock* next = nullptr;
	};

	HeapAllocator() = default;
	HeapAllocator(void* memory, size_t size);
	HeapAllocator(Allocator& parent, size_t size);

	virtual ~HeapAllocator();

	// Allocate raw aligned memory
	//
	void* Allocate(size_t size, size_t alignment) override;

	// Deallocate memory from this heap
	//
	void Deallocate(void* ptr) override;

	// Return true if there are no outstanding allocations
	//
	bool IsEmpty() const;

	// Calculate the total amount of free space left in the heap
	//
	size_t CalculateFreeSpace() const;

	// Head of the free list
	//
	FreeBlock* head = nullptr;
};
