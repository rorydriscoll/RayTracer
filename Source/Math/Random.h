#pragma once

#include <Math/Vector.h>

namespace Random
{
	// Set the seed
	//
	void SetSeed(int seed);

	// Int in the range [min, max)
	//
	int Integer(int min, int max);

	// Real in the range [0, 1]
	//
	float Real();

	// Point on the unit sphere
	//
	float3 PointOnSphere();
};
