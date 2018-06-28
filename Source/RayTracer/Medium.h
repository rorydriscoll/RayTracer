#pragma once

#include <Math/Vector.h>
#include <Core/Lut.h>

struct Allocator;

struct Medium
{
	Medium(float3 beta, float density_scale_height);

	// Helper to precompute the optical depth look up table
	//
	void PrecomputeOpticalDepth(int steps);

	// Helper to precompute inscattering from the given light direction
	//
	void PrecomputeInscattering(float3 l, float altitude, int steps, const Medium& rayleigh, const Medium& mie);

	// Return the transmittance from the light to b, and from b to a
	//
	float3 CalculateTransmittance(float ha, float angle_a_to_b, float hb, float angle_b_to_l) const;

	// Return the medium density at the given height
	//
	float CalculateDensity(float height) const;

	// Scattering coefficient (m^-1)
	//
	float3 beta = { 0, 0, 0 };

	// The height at which density goes down by 1/e (m)
	//
	float density_height = 0;

	// Height of the atmosphere (m)
	//
	float atmosphere_height = 0;

	// Lookup table by height (m) and zenith angle (rad) to optical depth (m)
	//
	Lut<float, 50, 90> depth;

	// Lookup table by distance (m) and view zenith angle (rad) to inscattering (1)
	//
	Lut<float3, 90> inscattering;
};
