#pragma once

#include <RayTracer/Brdf/Brdf.h>
#include <RayTracer/Brdf/FireflyReduction.h>

struct UberBrdf : Brdf
{
	UberBrdf(float3 normal, float3 albedo, float3 reflectance, float roughness) : 
		normal(normal), albedo(albedo), reflectance(reflectance), alpha(FireflyReduction::GetRoughness(Square(roughness)))
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

	// Diffuse layer albedo
	//
	float3 albedo = { 0, 0, 0 };

	// Specular layer normal reflectance
	//
	float3 reflectance = { 0, 0, 0 };

	// Specular layer alpha
	//
	float alpha = 0;
};
