#pragma once

#include <Math/Vector.h>

struct Ray
{
	Ray() = default;
	Ray(float3 p, float3 d) : p(p), d(d) {}

	// Origin point
	//
	float3 p = { 0, 0, 0 };

	// Positive direction
	//
	float3 d = { 1, 0, 0 };
};