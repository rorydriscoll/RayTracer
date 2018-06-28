#pragma once

#include "Allocator.h"

struct StackAllocator : Allocator
{
	StackAllocator() = default;
	StackAllocator(void* memory, size_t capacity);
	StackAllocator(Allocator& parent, size_t capacity);

	// Reset back to empty without freeing any allocations
	//
	void Clear();

	// Allocate raw aligned memory
	//
	void* Allocate(size_t size, size_t alignment) override;

	// Deallocate memory
	//
	void Deallocate(void* memory) override;

	// Return the number of bytes free
	//
	size_t BytesFree() const;

	// The current write point
	//
	void* head = nullptr;
};