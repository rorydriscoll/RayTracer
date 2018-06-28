#pragma once

#include <RayTracer/Texture/Texture.h>

template<typename T>
struct ConstantTexture : Texture<T>
{
	ConstantTexture(T value) : value(value) {}

	T Sample(float2 uv) const override
	{
		unused(uv);
		return value;
	}

	// Value to return
	//
	T value = T();
};
