#pragma once

#include <RayTracer/Brdf/Brdf.h>
#include <RayTracer/Brdf/UberBrdf.h>

struct MicrofacetBrdf : Brdf
{
	MicrofacetBrdf(float3 normal, float3 reflectance, float roughness) : normal(normal), reflectance(reflectance), alpha(FireflyReduction::GetRoughness(Square(roughness)))
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

	// Normal reflectance
	//
	float3 reflectance = { 1, 1, 1 };

	// Roughness
	//
	float alpha = 0.5f;
};
