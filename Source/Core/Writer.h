#pragma once

#include <Core/Types.h>
#include <new>

struct Writer
{
	Writer() = default;
	Writer(void* memory, size_t size);

	// Seek using an absolute offset
	//
	void Seek(size_t offset);

	// Seek using a relative offset
	//
	void Skip(size_t offset);

	// Reserve space and return the buffer pointer
	//
	void* Reserve(size_t size, size_t alignment);

	template<typename T> T* Reserve(int count = 1)
	{
		return reinterpret_cast<T*>(Reserve(sizeof(T) * count, __alignof(T)));
	}

	// Write a blob of data and return the new data pointer
	//
	void* Write(const void* value, size_t size, size_t alignment = 4);

	// Basic types
	//
	void Write(int8_t value);
	void Write(uint8_t value);
	void Write(int16_t value);
	void Write(uint16_t value);
	void Write(int32_t value);
	void Write(uint32_t value);
	void Write(uint64_t value);
	void Write(float value);

	template<typename T> void Write(const T* items, int count)
	{
		reinterpret_cast<T*>(Write(items, count * sizeof(T), __alignof(T)));
	}

	template<typename T, int N> void Write(T(&values)[N])
	{
		reinterpret_cast<T*>(Write(values, N * sizeof(T), __alignof(T)));
	}

	template<typename T, typename... Args> T* Create(Args&&... args)
	{
		return new (Reserve(sizeof(T), __alignof(T))) T(args...);
	}

	// String
	//
	void WriteString(const char* format, ...);

	// Helpers
	//
	size_t BytesWritten() const;
	size_t BytesRemaining() const;

	// Buffer pointers
	//
	uint8_t* begin = nullptr;
	uint8_t* head = nullptr;
	uint8_t* end = nullptr;
};
