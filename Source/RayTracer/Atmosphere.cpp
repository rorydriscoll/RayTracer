#include <RayTracer/Atmosphere.h>
#include <Math/Scalar.h>
#include <Core/Constants.h>
#include <Core/Log.h>

namespace
{
	// Solar illuminance in space (lux) https://en.wikipedia.org/wiki/Sunlight
	//
	const float solar_illuminance = 132276;

	// Height at which the rayleigh density drops by 1/e (m)
	//
	const float rayleigh_density_height = 8000;

	// Rayleigh scattering coefficient (m^-1)
	//
	const float3 rayleigh_beta = { 5.8e-6f, 13.5e-6f, 33.1e-6f };

	// Height at which the mie density drops by 1/e (m)
	//
	const float mie_density_height = 1200;

	// Mie scattering coefficient
	//
	const float3 mie_beta = { 2e-5f, 2e-5f, 2e-5f };

	// Number of integration steps to perform
	//
	const int integration_steps = 50;
}

Atmosphere::Atmosphere(float3 l, float haze, float eccentricity, float altitude) :
	rayleigh(rayleigh_beta, rayleigh_density_height),
	mie(mie_beta * haze, mie_density_height),
	eccentricity(eccentricity),
	altitude(altitude)
{
	// Precompute both optical depth tables first. The inscattering computations require
	// that all media have proper optical depth information first.

	rayleigh.PrecomputeOpticalDepth(integration_steps);
	mie.PrecomputeOpticalDepth(integration_steps);

	rayleigh.PrecomputeInscattering(l, altitude, integration_steps, rayleigh, mie);
	mie.PrecomputeInscattering(l, altitude, integration_steps, rayleigh, mie);

	sun.direction = l;
	sun.irradiance = CalculateExtinction(l) * solar_illuminance;

	LOG_INFO("Averate sky luminance is %.0f cd/m^2", CalculateLuminance(CalculateAverageLuminance()));
}

float3 Atmosphere::CalculateInscattering(float3 d) const
{
	// Apply the phase functions to the precomputed inscattering values. As noted in the Elek paper, this
	// isn't quite accurate, but the difference is negligable.
	//
	// As in the paper, this uses their modified Rayleigh phase function since it deals with the deferred
	// computation of the phase a bit better than the normal one.
	//
	// The mie phase function is the Cornette-Shanks improvement of Henyey-Greenstein. Positive eccentricity
	// values indicate forward scattering.

	const float u = Dot(d, sun.direction);
	const float g = eccentricity;

	const float gg = g * g;
	const float uu = u * u;
	const float gu = g * u;

	const float pm = 1.5f * (1 - gg) * (1 + uu) / ((2 + gg) * Pow(1 + gg - 2 * gu, 1.5f));
	const float pr = 0.8f * (1.2f + u * 0.5f);

	const float theta = ACos(d.y);

	// According to https://www.velux.com/deic/daylight/daylight-calculations-and-measurements the
	// average clear sky luminance 8000 cd/m^2. The average luminance for this model at noon is
	// 3948 cd/m^2. This adjustment factor puts the sky at about the right brightness.
	//
	// So why is the atmosphere model off by so much? My current theory is that it's due to missing
	// multiple scattering events. We're handling inscattering from the sun, but not from other 
	// out-scattered particles.

	const float adjustment = 2;

	return adjustment * (pr * rayleigh.inscattering.Sample(theta) + pm * mie.inscattering.Sample(theta));
}

float3 Atmosphere::CalculateExtinction(float3 d) const
{
	// This calculates extinction from the ground to the edge of the atmosphere in the supplied direction

	const float theta = ACos(d.y);

	return Exp(-(rayleigh.beta * rayleigh.depth.Sample(altitude, theta) + mie.beta * mie.depth.Sample(altitude, theta)));
}

float3 Atmosphere::CalculateAverageLuminance() const
{
	const int strata = 16;

	const float scale = 1.0f / strata;
	const float dw = scale * scale;

	float3 luminance = { 0, 0, 0 };

	for (int y = 0; y <= strata; ++y)
	{
		const float v = y * scale;

		for (int x = 0; x <= strata; ++x)
		{
			const float u = x * scale;

			const float phi = 2 * pi * v;

			const float cos_theta = u;
			const float sin_theta = Sqrt(1.0f - u * u);

			const float cos_phi = Cos(phi);
			const float sin_phi = Sin(phi);

			luminance += CalculateInscattering(float3(cos_phi * sin_theta, cos_theta, sin_phi * sin_theta)) * sun.irradiance * dw;
		}
	}

	return luminance;
}
