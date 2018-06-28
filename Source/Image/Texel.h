#pragma once

#include <Math/Vector.h>
#include <Core/Types.h>

struct Bgr
{
	Bgr() = default;

	Bgr(uint8_t b, uint8_t g, uint8_t r) : b(b), g(g), r(r)
	{
	}

	Bgr(float3 linear) :
		b(uint8_t(LinearToGamma(linear.b) * 255)),
		g(uint8_t(LinearToGamma(linear.g) * 255)),
		r(uint8_t(LinearToGamma(linear.r) * 255))
	{
	}

	float3 ToLinear() const
	{
		return { GammaToLinear(r / 255.0f), GammaToLinear(g / 255.0f), GammaToLinear(b / 255.0f) };
	}

	uint8_t b = 0;
	uint8_t g = 0;
	uint8_t r = 0;
};

struct Bgra
{
	Bgra() = default;
	
	Bgra(uint8_t b, uint8_t g, uint8_t r) : b(b), g(g), r(r)
	{
	}

	Bgra(float3 linear) :
		b(uint8_t(LinearToGamma(linear.b) * 255)),
		g(uint8_t(LinearToGamma(linear.g) * 255)),
		r(uint8_t(LinearToGamma(linear.r) * 255))
	{
	}

	float3 ToLinear() const
	{
		return { GammaToLinear(r / 255.0f), GammaToLinear(g / 255.0f), GammaToLinear(b / 255.0f) };
	}

	uint8_t b = 0;
	uint8_t g = 0;
	uint8_t r = 0;
	uint8_t a = 0;
};
