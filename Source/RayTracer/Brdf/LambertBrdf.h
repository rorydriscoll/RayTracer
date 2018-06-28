#pragma once

#include <RayTracer/Brdf/Brdf.h>

struct LambertBrdf : Brdf
{
	LambertBrdf(float3 normal, float3 albedo) : normal(normal), albedo(albedo)
	{
	}

	// Evaluate the cosine-weighted brdf
	//
	float3 Evaluate(float3 l, float3 v) const override;

	// Sample the cosine-weighted brdf for both the incoming light direction and sample weight
	//
	float3 Sample(float3& l, float3 v, float2 sample) const override;

	// World-space normal
	//
	float3 normal = { 0, 0, 1 };

	// Reflectance
	//
	float3 albedo = { 1, 1, 1 };
};
