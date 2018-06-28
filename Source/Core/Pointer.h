#pragma once

#include <Core/Assert.h>
#include <Core/Types.h>
#include <cstdint>

namespace Pointer
{
	template<typename T>
	T* Align(T* address, size_t alignment)
	{
		ASSERT((alignment & (alignment - 1)) == 0, "Alignment is not a power of two");

		return reinterpret_cast<T*>((uintptr_t(address) + (alignment - 1)) & ~(alignment - 1));
	}

	template<typename T>
	T* Offset(T* address, ptrdiff_t offset)
	{
		return reinterpret_cast<T*>((uintptr_t(address) + offset));
	}

	template<typename T, typename U>
	ptrdiff_t Difference(T* a, U* b)
	{
		return intptr_t(b) - intptr_t(a);
	}

	inline size_t CalculateAdjustment(void* address, size_t header, size_t alignment)
	{
		ASSERT((alignment & (alignment - 1)) == 0, "Alignment is not a power of two");

		const uintptr_t begin = uintptr_t(address);
		const uintptr_t adjusted = ((begin + header) + (alignment - 1)) & ~(alignment - 1);

		return adjusted - begin;
	}
}