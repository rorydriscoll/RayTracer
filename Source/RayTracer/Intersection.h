#pragma once

#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Core/Constants.h>

struct Material;

struct Intersection
{
	float3x3 CalculateTransform() const
	{
		return {Normalize(dpdu), Normalize(dpdv), normal};
	}

	// Hit time
	//
	float t = max_float_value;

	// Hit point
	//
	float3 point = { 0, 0, 0 };

	// Surface normal
	//
	float3 normal = { 0, 0, 1 };

	// Position derivatives
	//
	float3 dpdu = { 1, 0, 0 };
	float3 dpdv = { 0, 1, 0 };

	// Texture coordinates
	//
	float2 uv = { 0, 0 };

	// Hit material
	//
	const Material* material = nullptr;
};
