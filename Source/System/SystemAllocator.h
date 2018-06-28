#pragma once

#include <Core/HeapAllocator.h>

struct SystemAllocator : HeapAllocator
{
	SystemAllocator(size_t size);
	~SystemAllocator();
};