#pragma once

#include <RayTracer/Integrator/Integrator.h>

struct DirectIntegrator : Integrator
{
	DirectIntegrator() = default;

	// Return the radiance for the ray
	//
	float3 Li(Sampler& sampler, Ray ray, const Scene& scene) const override;
};
