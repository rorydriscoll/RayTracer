#include <Image/Pfm.h>
#include <System/File.h>
#include <Core/Constants.h>
#include <Core/Log.h>
#include <Core/Writer.h>
#include <string>

namespace
{
	const char* ParseInt(int& value, const char* begin)
	{
		const char* end = begin;

		while (*end != '\n')
			end++;

		value = std::stoi(begin);

		return end + 1;
	}

	const char* ParseFloat(float& value, const char* begin)
	{
		const char* end = begin;

		while (*end != '\n')
			end++;

		value = std::stof(begin);

		return end + 1;
	}
}

bool Pfm::LoadImage(Image<float3>& image, Allocator& allocator, const void* buffer)
{
	const char* head = reinterpret_cast<const char*>(buffer);

	if (head[0] != 'P' || head[1] != 'F' || head[2] != '\n')
	{
		LOG_ERROR("Bad pfm header");
		return false;
	}

	head += 3;

	int w, h;

	head = ParseInt(w, head);
	head = ParseInt(h, head);

	if (w < 0 || h < 0)
	{
		LOG_ERROR("Bad dimensions");
		return false;
	}

	float aspect;

	head = ParseFloat(aspect, head);

	if (aspect != -1) // -ve indicates that the values are stored in little-endian
	{
		LOG_ERROR("Bad pixel aspect");
		return false;
	}

	image = Image<float3>(allocator, w, h);

	image.Fill(reinterpret_cast<const float3*>(head));

	return true;
}

bool Pfm::LoadImage(Image<float3>& image, Allocator& allocator, const char* path)
{
	File file;

	if (!file.OpenForRead(path))
		return false;

	return LoadImage(image, allocator, file.contents);
}

bool Pfm::SaveImage(const char* path, const Image<float3>& image)
{
	// Require 4 bytes per channel x 3 channels x w x h for texel data. We need a little
	// bit more for the header which is plain-text and so is variable size.

	const size_t total_required = sizeof(float) * 3 * image.w * image.h + 1_KiB;

	File file;

	if (!file.OpenForWrite(path, total_required))
	{
		LOG_ERROR("Failed to save image to %s", path);
		return false;
	}

	Writer writer(file.contents, file.size);

	writer.WriteString("PF\n");
	writer.WriteString("%i\n", image.w);
	writer.WriteString("%i\n", image.h);
	writer.WriteString("-1.0\n"); // Aspect ratio and -ve indicates little endian

	writer.Write(image.texels, image.w * image.h);

	return true;
}
