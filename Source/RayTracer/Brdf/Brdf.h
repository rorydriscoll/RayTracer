#pragma once

#include <Math/Vector.h>

struct Brdf
{
	Brdf() = default;

	virtual ~Brdf() = default;

	// Evaluate the cosine-weighted brdf
	//
	virtual float3 Evaluate(float3 l, float3 v) const = 0;

	// Sample the cosine-weighted brdf for both the incoming light direction and sample weight
	//
	virtual float3 Sample(float3& l, float3 v, float2 sample) const = 0;
};
