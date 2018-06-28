#pragma once

#include <RayTracer/Texture/Texture.h>
#include <Image/Image.h>
#include <Image/Tga.h>
#include <Core/Generic.h>

struct LinearValue
{
	using Compressed = uint8_t;
	using Decompressed = float;

	static Decompressed Decompress(Compressed t)
	{
		return t / 255.0f;
	}
};

struct GammaColor
{
	using Compressed = Bgr;
	using Decompressed = float3;

	static Decompressed Decompress(Compressed t)
	{
		return t.ToLinear();
	}
};

struct Linear3
{
	using Compressed = Bgr;
	using Decompressed = float3;

	static Decompressed Decompress(Compressed t)
	{
		return { t.r / 255.0f, t.g / 255.0f, t.b / 255.0f };
	}
};

template<typename T>
struct ImageTexture : Texture<typename T::Decompressed>
{
	ImageTexture(Allocator& allocator, const char* path)
	{
		const bool loaded = Tga::LoadImage(image, allocator, path);

		CRITICAL(loaded, "Image not loaded");
	}

	typename T::Decompressed Sample(float2 uv) const override
	{
		const float s = Floor(Wrap(uv.x * image.w, float(image.w)));
		const float t = Floor(Wrap(uv.y * image.h, float(image.h)));

		const int i0 = int(s);
		const int i1 = (i0 + 1) % image.w;
		const int j0 = int(t);
		const int j1 = (j0 + 1) % image.h;

		const float sr = Fract(s);
		const float tr = Fract(t);

		const typename T::Decompressed t00 = T::Decompress(image.texels[j0 * image.w + i0]);
		const typename T::Decompressed t01 = T::Decompress(image.texels[j0 * image.w + i1]);
		const typename T::Decompressed t10 = T::Decompress(image.texels[j1 * image.w + i0]);
		const typename T::Decompressed t11 = T::Decompress(image.texels[j1 * image.w + i1]);

		return Lerp(Lerp(t00, t01, sr), Lerp(t10, t11, sr), tr);
	}

	Image<typename T::Compressed> image;
};
