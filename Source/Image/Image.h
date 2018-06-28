#pragma once

#include <Core/Assert.h>
#include <Core/Allocator.h>

template<typename T>
struct Image
{
	Image() = default;
	Image(Allocator& allocator, int w, int h) : allocator(&allocator), texels(allocator.NewArray<T>(w * h)), w(w), h(h) {}

	Image(const Image<T>&) = delete;
	Image& operator=(const Image<T>&) = delete;

	Image(Image<T>&& other) noexcept : allocator(other.allocator), texels(other.texels), w(other.w), h(other.h)
	{
		other.allocator = nullptr;
		other.texels = nullptr;
		other.w = other.h = 0;
	}

	Image& operator=(Image<T>&& other) noexcept
	{
		if (allocator)
			allocator->DeleteArray(texels, w * h);

		allocator = other.allocator;
		texels = other.texels;
		w = other.w;
		h = other.h;

		other.allocator = nullptr;
		other.texels = nullptr;
		other.w = other.h = 0;

		return *this;
	}

	~Image()
	{
		if (allocator)
			allocator->DeleteArray(texels, w * h);
	}

	T& operator()(int x, int y)
	{
		ASSERT(x >= 0 && x < w && y >= 0 && y < h);
		return texels[y * w + x];
	}

	const T& operator()(int x, int y) const
	{
		ASSERT(x >= 0 && x < w && y >= 0 && y < h);
		return texels[y * w + x];
	}

	T& operator[](int index)
	{
		ASSERT(index >= 0 && index < w * h);
		return texels[index];
	}

	const T& operator[](int index) const
	{
		ASSERT(index >= 0 && index < w * h);
		return texels[index];
	}

	void Fill(T item)
	{
		const int count = w * h;

		for (int i = 0; i < count; ++i)
			texels[i] = item;
	}

	void Fill(const T* texels_)
	{
		const int count = w * h;

		for (int i = 0; i < count; ++i)
			texels[i] = texels_[i];
	}

	// Allocator used
	//
	Allocator* allocator = nullptr;

	// Image texels
	//
	T* texels = nullptr;

	// Dimensions
	//
	int w = 0, h = 0;
};
