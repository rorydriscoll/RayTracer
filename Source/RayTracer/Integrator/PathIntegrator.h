#pragma once

#include <RayTracer/Integrator/Integrator.h>

struct PathIntegrator : Integrator
{
	PathIntegrator(int max_depth) : max_depth(max_depth) {}

	// Return the radiance for the ray
	//
	float3 Li(Sampler& sampler, Ray ray, const Scene& scene) const override;

	// Max ray depth
	//
	int max_depth = 3;
};
