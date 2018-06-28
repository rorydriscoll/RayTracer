#include <Image/Tga.h>
#include <System/File.h>
#include <Core/Types.h>
#include <Core/Log.h>
#include <Core/Writer.h>

namespace
{
	bool IsHeaderValid(const Tga::Header* header, const int bits)
	{
		// Not supporting ident

		if (header->ident_size != 0)
		{
			LOG_ERROR("TGA ident size is not zero");
			return false;
		}

		// Image must not be palletized

		if (header->color_map_type != 0)
		{
			LOG_ERROR("TGA cannot be palletized");
			return false;
		}

		// Image must be RGB

		if (header->image_type != 2 && header->image_type != 3)
		{
			LOG_ERROR("TGA must be RGB or grayscale");
			return false;
		}

		// Dimensions must be positive

		if (header->width < 0 || header->height < 0)
		{
			LOG_ERROR("TGA width must be positive");
			return false;
		}

		// Must be four channels

		if (header->bits != bits)
		{
			LOG_ERROR("TGA bit depth must be 32");
			return false;
		}

		// No support for horzontal flip

		if (header->descriptor == 0x10)
		{
			LOG_ERROR("Unknown TGA descriptor 0x%02x", header->descriptor);
			return false;
		}

		return true;
	}

	enum class RowOrder
	{
		BottomToTop,
		TopToBottom
	};

	template<typename T>
	void LoadTexels(T* target, const T* source, int w, int h, RowOrder input_order)
	{
		if (input_order == RowOrder::TopToBottom)
		{
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					const int s = y * w + x;
					const int t = y * w + x;

					target[t] = source[s];
				}
			}
		}
		else
		{
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					const int s = (h - y - 1) * w + x;
					const int t = y * w + x;

					target[t] = source[s];
				}
			}
		}
	}

	template<typename T>
	bool LoadImage(Image<T>& image, Allocator& allocator, const void* buffer)
	{
		const Tga::Header* header = static_cast<const Tga::Header*>(buffer);

		if (!IsHeaderValid(header, sizeof(T) * 8))
			return false;

		image = Image<T>(allocator, header->width, header->height);

		if (image.w == 0 && image.h == 0)
			return true;

		const RowOrder order = header->descriptor & 0x20 ? RowOrder::TopToBottom : RowOrder::BottomToTop;

		LoadTexels(image.texels, reinterpret_cast<const T*>(header + 1), image.w, image.h, order);

		return true;
	}

	template<typename T>
	bool LoadImage(Image<T>& image, Allocator& allocator, const char* filename)
	{
		File file;

		if (!file.OpenForRead(filename))
			return false;

		return LoadImage(image, allocator, file.contents);
	}
}

bool Tga::LoadImage(Image<uint8_t>& image, Allocator& allocator, const char* filename)
{
	return ::LoadImage(image, allocator, filename);
}

bool Tga::LoadImage(Image<uint8_t>& image, Allocator& allocator, const void* buffer)
{
	return ::LoadImage(image, allocator, buffer);
}

bool Tga::LoadImage(Image<Bgr>& image, Allocator& allocator, const char* filename)
{
	return ::LoadImage(image, allocator, filename);
}

bool Tga::LoadImage(Image<Bgr>& image, Allocator& allocator, const void* buffer)
{
	return ::LoadImage(image, allocator, buffer);
}

bool Tga::LoadImage(Image<Bgra>& image, Allocator& allocator, const char* filename)
{
	return ::LoadImage(image, allocator, filename);
}

bool Tga::LoadImage(Image<Bgra>& image, Allocator& allocator, const void* buffer)
{
	return ::LoadImage(image, allocator, buffer);
}

bool Tga::SaveImage(const char* path, const Image<Bgra>& image)
{
	const size_t stride = 4;
	const size_t size = sizeof(Header) + image.w * image.h * stride;

	File file;

	if (!file.OpenForWrite(path, size))
		return false;

	Writer writer(file.contents, file.size);

	Header* header = writer.Create<Header>();

	header->ident_size = 0;
	header->color_map_type = 0;
	header->image_type = 2;
	header->color_map_start = 0;
	header->color_map_length = 0;
	header->color_map_bits = 0;
	header->x_start = 0;
	header->y_start = 0;
	header->width = int16_t(image.w);
	header->height = int16_t(image.h);
	header->bits = stride * 8;
	header->descriptor = 0x20; // Top to bottom row order

	writer.Write(image.texels, image.w * image.h);

	file.Close();

	return true;
}
