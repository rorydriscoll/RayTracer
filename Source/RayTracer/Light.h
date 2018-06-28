#pragma once

#include <Math/Vector.h>

struct Light
{
	Light() = default;
	Light(float3 direction, float3 irradiance) : direction(direction), irradiance(irradiance) {}

	// Direction to the light
	//
	float3 direction = { 0, 1, 0 };

	// Irradiance
	//
	float3 irradiance = { 1, 1, 1 };
};
