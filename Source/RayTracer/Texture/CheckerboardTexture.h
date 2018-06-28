#pragma once

#include <RayTracer/Texture/Texture.h>
#include <Core/Constants.h>

template<typename T>
struct CheckerboardTexture : Texture<T>
{
	CheckerboardTexture(T a, T b, float fu, float fv) : a(a), b(b), fu(fu), fv(fv) {}

	T Sample(float2 uv) const override
	{
		return Cos(uv.x * fu * tau) * Cos(uv.y * fv * tau) < 0 ? a : b;
	}

	// Colors to alternate
	//
	T a = T();
	T b = T();

	// Frequencies
	//
	float fu = 1;
	float fv = 1;
};