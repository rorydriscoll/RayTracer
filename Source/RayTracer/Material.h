#pragma once

#include <RayTracer/Texture/Texture.h>
#include <RayTracer/Brdf/UberBrdf.h>
#include <Math/Matrix.h>

struct Intersection;

struct Material
{
	Material() = default;

	Material(const Texture<float3>& normal, const Texture<float3>& color, const Texture<float>& roughness, const Texture<float>& metalness) :
		normal(&normal), color(&color), roughness(&roughness), metalness(&metalness)
	{
	}

	// Create a brdf for the given position in uv-space
	//
	UberBrdf CreateBrdf(float2 uv, float3x3 world_from_surface) const;

	// Compressed tangent-space normal map
	//
	const Texture<float3>* normal = nullptr;

	// Base color
	//
	const Texture<float3>* color = nullptr;

	// Roughness
	//
	const Texture<float>* roughness = nullptr;

	// Metalness
	//
	const Texture<float>* metalness = nullptr;
};
