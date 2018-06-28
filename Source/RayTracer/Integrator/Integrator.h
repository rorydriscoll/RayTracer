#pragma once

#include <Math/Vector.h>

struct Sampler;
struct Ray;
struct Scene;

struct Integrator
{
	virtual ~Integrator() {}

	// Return the radiance for the ray
	//
	virtual float3 Li(Sampler& sampler, Ray ray, const Scene& scene) const = 0;
};
