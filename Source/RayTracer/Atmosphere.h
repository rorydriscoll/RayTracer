#pragma once

#include <RayTracer/Medium.h>
#include <RayTracer/Light.h>
#include <Math/Vector.h>

// Implements atmospheric scattering by Oskar Elek:
//
// http://old.cescg.org/CESCG-2009/papers/PragueCUNI-Elek-Oskar09.pdf

struct Allocator;

struct Atmosphere
{
	Atmosphere(float3 l, float haze, float eccentricity, float altitude);

	// Calculate inscattering factor in the supplied direction (1)
	//
	float3 CalculateInscattering(float3 d) const;

	// Calculate extinction factor in the supplied direction (1)
	//
	float3 CalculateExtinction(float3 d) const;

	// Return the average luminance
	//
	float3 CalculateAverageLuminance() const;

	// Inscattering lookup tables (view angle (rad)) -> instcattering (1)
	//
	Medium rayleigh;
	Medium mie;

	// Mie eccentricity parameter (controls anisotropy) (-1, 1)
	//
	float eccentricity = 0;

	// Altitude to sample from
	//
	float altitude = 0;

	// Sun directional light
	//
	Light sun;
};
