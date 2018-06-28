#pragma once

#include <Core/Allocator.h>
#include <Core/Assert.h>

template<typename T>
struct Array
{
	Array() = default;
	Array(Allocator& allocator, int count) : allocator(&allocator), values(allocator.NewArray<T>(count)), count(count) {}

	Array(const Array&) = delete;
	void operator=(const Array&) = delete;

	Array(Array&& other) noexcept : allocator(other.allocator), values(other.values), count(other.count)
	{
		other.allocator = nullptr;
		other.values = nullptr;
		other.count = 0;
	}

	Array& operator=(Array&& other) noexcept
	{
		if (allocator)
			allocator->DeleteArray(values, count);

		allocator = other.allocator;
		values = other.values;
		count = other.count;

		other.allocator = nullptr;
		other.values = nullptr;
		other.count = 0;

		return *this;
	}

	~Array()
	{
		if (allocator)
			allocator->DeleteArray(values, count);
	}

	T& operator[](int index)
	{
		ASSERT(index >= 0 && index < count);
		return values[index];
	}

	const T& operator[](int index) const
	{
		ASSERT(index >= 0 && index < count);
		return values[index];
	}

	// Allocator used
	//
	Allocator* allocator = nullptr;

	// All values
	//
	T* values = nullptr;

	// Number of values
	//
	int count = 0;
};
