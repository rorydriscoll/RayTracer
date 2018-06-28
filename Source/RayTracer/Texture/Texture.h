#pragma once

#include <Math/Vector.h>

template<typename T>
struct Texture
{
	virtual ~Texture() = default;

	// Sample the texture at the given normalized coordinates
	//
	virtual T Sample(float2 uv) const = 0;
};

