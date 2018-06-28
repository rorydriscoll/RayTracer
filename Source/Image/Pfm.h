#pragma once

#include <Image/Image.h>
#include <Math/Vector.h>

namespace Pfm
{
	// Load an image from memory or from file
	//
	bool LoadImage(Image<float3>& image, Allocator& allocator, const void* buffer);
	bool LoadImage(Image<float3>& image, Allocator& allocator, const char* path);
		
	// Save an image to Portable Float Map format
	//
	bool SaveImage(const char* path, const Image<float3>& image);
}
