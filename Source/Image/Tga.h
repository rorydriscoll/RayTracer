#pragma once

#include <Image/Texel.h>
#include <Image/Image.h>

namespace Tga
{
	#pragma pack(push, 1)

	struct Header
	{
		uint8_t ident_size;        // size of ID field that follows 18 byte header (0 usually)
		uint8_t color_map_type;    // type of color map 0=none, 1=has palette
		uint8_t image_type;        // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

		int16_t color_map_start;   // first color map entry in palette
		int16_t color_map_length;  // number of colours in palette
		uint8_t color_map_bits;    // number of bits per palette entry 15,16,24,32

		int16_t x_start;           // image x origin
		int16_t y_start;           // image y origin
		int16_t width;             // image width in pixels
		int16_t height;            // image height in pixels
		uint8_t bits;              // image bits per pixel 8,16,24,32
		uint8_t descriptor;        // image descriptor bits (vh flip bits)
	};

	#pragma pack(pop)

	// Load from 8 bpp tga
	//
	bool LoadImage(Image<uint8_t>& image, Allocator& allocator, const void* buffer);
	bool LoadImage(Image<uint8_t>& image, Allocator& allocator, const char* filename);

	// Load from 24 bpp tga
	//
	bool LoadImage(Image<Bgr>& image, Allocator& allocator, const void* buffer);
	bool LoadImage(Image<Bgr>& image, Allocator& allocator, const char* filename);

	// Load from 32 bpp tga
	//
	bool LoadImage(Image<Bgra>& image, Allocator& allocator, const void* buffer);
	bool LoadImage(Image<Bgra>& image, Allocator& allocator, const char* filename);

	// Save to tga on disk
	//
	bool SaveImage(const char* path, const Image<Bgra>& image);
}
