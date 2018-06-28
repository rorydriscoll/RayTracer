#pragma once

#include <RayTracer/Integrator/Integrator.h>

struct DepthIntegrator : Integrator
{
	DepthIntegrator() = default;
	DepthIntegrator(float depth_scale) : depth_scale(depth_scale) {}

	// Return the radiance for the ray
	//
	float3 Li(Sampler& sampler, Ray ray, const Scene& scene) const override;

	// Depth at which the output is one
	//
	float depth_scale = 0.05f;
};
