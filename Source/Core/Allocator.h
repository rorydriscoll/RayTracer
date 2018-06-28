#pragma once

struct Allocator
{
	Allocator() = default;
	Allocator(void* memory, size_t capacity);
	Allocator(Allocator& parent, size_t capacity);

	Allocator(const Allocator&) = delete;
	Allocator& operator=(const Allocator&) = delete;

	Allocator(Allocator&&) noexcept;
	Allocator& operator=(Allocator&&) noexcept;

	virtual ~Allocator();

	template<typename T> T* New()
	{
		return new (Allocate(sizeof(T), alignof(T))) T();
	}

	template<typename T> T* NewArray(int count)
	{
		T* objects = reinterpret_cast<T*>(Allocate(sizeof(T) * count, alignof(T)));

		for (int i = 0; i < count; ++i)
			new (objects + i) T();

		return objects;
	}

	template<typename T> void Delete(T* object)
	{
		if (!object)
			return;

		object->~T();
		Deallocate(object);
	}

	template<typename T> void DeleteArray(T* objects, int count)
	{
		if (!objects)
			return;

		for (int i = 0; i < count; ++i)
			objects[i].~T();

		Deallocate(objects);
	}

	// Allocate raw aligned memory
	//
	virtual void* Allocate(size_t size, size_t alignment) = 0;

	// Deallocate memory
	//
	virtual void Deallocate(void* allocation) = 0;

	// Allocator this allocator used to allocate its memory
	//
	Allocator* parent = nullptr;

	// Memory buffer
	//
	void* memory = nullptr;

	// Total number of bytes allocated excluding padding
	//
	size_t allocated = 0;

	// Total number of bytes available
	//
	size_t capacity = 0;
};
