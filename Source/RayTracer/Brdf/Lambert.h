#pragma once

#include <Math/Vector.h>
#include <Core/Constants.h>

namespace Lambert
{
	inline float3 Evaluate(float ndotl, float3 albedo)
	{
		// Energy-conserving Lambert

		return albedo * ndotl / pi;
	}

	// Calculate the light direction for importance sampling
	//
	float3 CalculateLightDirection(float3 n, float2 sample);
}
