#include <RayTracer/Medium.h>
#include <Math/Scalar.h>
#include <Core/Constants.h>

namespace
{
	// Min altitude to calculate inscattering from. This should be above zero to avoid hitting the planet immediately (m)
	// 
	const float min_sample_height = 1;

	// Radius of the sphere for the planet (m)
	//
	const float planet_radius = 6371000;

	float CalculateAtmosphereHeight(float density_height)
	{
		// Each density_height meters, the value scales by 1/e. We'll pick a
		// cutoff value, then determine the max height from that.
		//
		// e^(-x / d) = cutoff
		// -x / d = ln(cutoff)
		// x = -d * ln(cutoff)

		const float cutoff = 0.001f; // Ignore the last 1/10th of a percent of atmosphere
		
		return -density_height * Ln(cutoff);
	}

	float CalculateHeight(float2 p)
	{
		return Length(p) - planet_radius;
	}

	float2 CalculateDirection(float cos_theta)
	{
		return { Sqrt(1 - cos_theta * cos_theta), cos_theta };
	}

	float CalculateDistanceToPlanet(float altitude, float cos_theta)
	{
		// At planet-scale, there's just not enough precision to correctly solve raycasts downwards
		// at the planet surface. We know they're going to immediately hit the ground, so just return so.

		if (altitude <= 0 && cos_theta < 0)
			return 0;

		// We're starting outside a circle, at point p, and ray casting in direction d until we
		// hit the circle or miss entirely

		// x = p + d * t
		// ||x|| = r
		//
		// x.x = r^2
		// (p + d * t).(p + d * t) = r^2
		// p.p + 2 * p.d * t + d.d * t^2 = r^2
		// d.d * t^2 + 2 * p.d * t + p.p - r^2 = 0
		//
		// Solve quadratic:
		//
		// a = d.d
		// b = 2 * p.d
		// c = p.p - r^2
		//
		// Our start point is:
		//
		// p = {0, h}
		//
		// Where h = planet_radius + altitude
		//
		// Our ray direction is
		//
		// d = {sqrt(1 - u * u), u}
		//
		// where u = cos_theta
		//
		// Note:
		//   d.d = 1
		//   p.d = h * u
		//   p.p = h * h
		//   r = planet_radius
		//
		// a = 1
		// b = 2 * h * u
		// c = h * h - r * r
		//
		// t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a)
		//   = (-2hu - sqrt(2hu * 2hu - 4 (hh - rr))) / 2
		//   = (-2hu - sqrt(4hhuu - 4hh + 4rr)) / 2
		//   = (-2hu - 2 * sqrt(hhuu - hh + rr)) / 2
		//   = (-hu - sqrt(hhuu - hh + rr))

		const float h = planet_radius + altitude;
		const float r = planet_radius;
		const float u = cos_theta;

		const float hh = h * h;
		const float hu = h * u;
		const float rr = r * r;

		const float discriminant = hu * hu - hh + rr;

		if (discriminant < 0)
			return max_float_value;

		const float t = -(hu + Sqrt(discriminant));

		return t < 0 ? max_float_value : t;
	}

	float CalculateDistanceToEdgeOfAtmosphere(float altitude, float cos_theta, float atmosphere_height)
	{
		// We're starting inside a circle, at point p, and ray casting out in direction d until we
		// exit the circle.

		// x = p + d * t
		// ||x|| = r
		//
		// x.x = r^2
		// (p + d * t).(p + d * t) = r^2
		// p.p + 2 * p.d * t + d.d * t^2 = r^2
		// d.d * t^2 + 2 * p.d * t + p.p - r^2 = 0
		//
		// Solve quadratic:
		//
		// a = d.d
		// b = 2 * p.d
		// c = p.p - r^2
		//
		// Our start point is:
		//
		// p = {0, rp + h}
		//
		// Our ray direction is
		//
		// d = {1 - cos_theta * cos_theta, cos_theta}
		//
		// Note:
		//   d.d = 1
		//   p.d = h * cos_theta
		//   p.p = h * h
		//   r = rp + ha + atmosphere_height

		const float h = planet_radius + altitude;
		const float r = planet_radius + atmosphere_height;
		const float u = cos_theta;
		const float hh = h * h;
		const float rr = r * r;
		const float uu = u * u;

		const float discriminant = hh * uu - hh + rr;

		if (discriminant < 0)
			return 0;

		const float t = Sqrt(discriminant) - h * u;

		return Max(0.0f, t);
	}

	float CalculateRayDistance(float altitude, float cos_theta, float atmosphere_height)
	{
		return Min(CalculateDistanceToPlanet(altitude, cos_theta), CalculateDistanceToEdgeOfAtmosphere(altitude, cos_theta, atmosphere_height));
	}
}

Medium::Medium(float3 beta, float density_scale_height) :
	beta(beta),
	density_height(density_scale_height),
	atmosphere_height(CalculateAtmosphereHeight(density_height)),
	depth(min_sample_height, atmosphere_height, 0, 90_deg),
	inscattering(0, 90_deg)
{
}

void Medium::PrecomputeOpticalDepth(int steps)
{
	// 2D lut where x is the sample point height in meters, and y is the angle of the view 
	// direction from the zenith in radians.
	//
	// This integrates the density function from the starting point at altitude h along the
	// view direction theta until we reach the edge of the atmosphere or we hit the planet
	// to get the final optical depth.

	for (int y : depth.y)
	{
		const float cos_theta = Cos(depth.y.CalculateValue(y));

		const float2 direction = CalculateDirection(cos_theta);

		for (int x : depth.x)
		{
			const float ha = depth.x.CalculateValue(x);
			const float ds = CalculateRayDistance(ha, cos_theta, atmosphere_height) / steps;

			const float2 a = { 0, planet_radius + ha };
			const float2 step = direction * ds;

			for (int i = 0; i < steps; ++i)
			{
				const float hb = CalculateHeight(a + step * i);

				depth(x, y) += CalculateDensity(hb) * ds;

				const float d = depth(x, y);
				ASSERT(!IsNan(d) && !IsInf(d));
			}
		}
	}
}

void Medium::PrecomputeInscattering(float3 l, float altitude, int steps, const Medium& rayleigh, const Medium& mie)
{
	// Probably the clearest explaination of what is going on here:
	//
	// https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/simulating-sky/simulating-colors-of-the-sky
	//
	// We're going to integrate the inscattering for every possible view direction from the zenith to the horizon. Note that 
	// the phase function is excluded from these calculations so that we don't need an extra table dimension. This causes some 
	// slight error, but it's negligable.
	//
	// For each direction, we want to calculate the inscattering along the ray to the edge of the atmosphere. The inscattered
	// light at a particular point b is the sun light received at that point, multiplied by the probablility it will scatter
	// from the light direction to the view direction (the phase function) and the scattering coefficient (beta). We have to 
	// take care of the fact that the light will be affected by extinction when going from the light to point b, and from 
	// point b to point a.
	//
	//            c         d
	//             \       /
	//              \     /
	//               \   /
	//                \ /    
	//                 b   _ v
	//                /    /|
	//               /    /
	//              a
	//
	// --------------------------------------------
	//
	// I(v) = Integrate(L(b) * P(v, l) * beta(b) * t(a, b) ds, a, d)             (1)
	//
	// The light received from the sun at a point b is:
	//
	// L(b) = L * t(p, c)
	//
	// The transmittance is calculated from the density function and scattering coefficient
	//
	// t(a, b) = Exp(-Integrate(beta(p) ds, a, b))
	//
	// The scattering coefficient decreases exponentially and is a maximum value of beta at sea level:
	//
	// beta(p) = beta * Exp(-height(p) / density_height)
	//
	// We can pull the scattering coefficient out of the integral and express transmittance as a function
	// of optical depth
	//
	// t(a, b) = Exp(-beta * Integrate(Exp(-height(p) / density_height) ds, a, b))
	//         = Exp(-beta * depth(a, b))
	//
	// Therefore:
	//
	// L(b) = L * Exp(-beta * depth(b, c))
	//
	// Substituting back into (1):
	//
	// I(v) = Integrate(L * Exp(-beta * depth(b, c)) * P(v, l) * beta(b) * t(a, b) ds, a, d) 
	//
	// We can remove some constants from the integration (assuming the light is so far away that l is
	// effectively constant):
	//
	// I(v) = L * P(v, l) * Integrate(Exp(-beta * depth(b, c)) * beta(b) * t(a, b) ds, a, d) 
	//
	// Continue to substitute in the value for beta(b):
	//
	// I(v) = L * P(v, l) * Integrate(Exp(-beta * depth(b, c)) * beta * Exp(-height(b) / density_height) * t(a, b) ds, a, d) 
	//      = L * P(v, l) * beta * Integrate(Exp(-beta * depth(b, c)) * Exp(-height(b) / density_height) * t(a, b) ds, a, d) 
	//
	// And similarly for the transmittance from a to b:
	//
	// I(v) = L * P(v, l) * beta * Integrate(Exp(-beta * depth(b, c)) * Exp(-height(b) / density_height) * Exp(-beta * depth(a, b)) ds, a, d) 
	//
	// These exponents can all be combined together to save a bit of computation
	//
	// I(v) = L * P(v, l) * beta * Integrate(Exp(-(beta * (depth(a, b) + depth(b, c)) + height(b) / density_height)) ds, a, d) 
	//
	// We will defer the multiplication of the phase function and illuminance until the pixel shader, so the inscattering
	// look up table will store:
	//
	// LUT(v) = beta * Integrate(Exp(-(beta * (depth(a, b) + depth(b, c)) + height(b) / density_height)) ds, a, d) 
	//
	// Note that the beta value outside the integral is the scattering coefficient for the current medium. The beta and depth
	// values inside the integral should take into consideration all media.
	//
	// To recover the inscattering, we calculate the phase function and apply the sun illuminance:
	//
	// I(v) = L * P(v, l) * LUT(v)

	for (int x : inscattering.x)
	{
		const float theta = inscattering.x.CalculateValue(x);
		const float cos_theta = Cos(theta);

		const float ha = altitude;
		const float ds = CalculateRayDistance(ha, cos_theta, atmosphere_height) / steps;

		const float2 a = { 0, planet_radius + ha };
		const float2 step = CalculateDirection(cos_theta) * ds;

		inscattering[x] = { 0, 0, 0 };

		for (int i = 0; i < steps; ++i)
		{
			// Calculate the integration point b

			const float2 b = a + step * i;
			const float hb = CalculateHeight(b);

			// Calculate the transmittance from a to b to c for both media

			const float cos_angle_a_to_b = cos_theta;
			const float cos_angle_b_to_l = Dot(Normalize(b), CalculateDirection(l.y));

			const float angle_a_to_b = ACos(cos_angle_a_to_b);
			const float angle_b_to_l = ACos(cos_angle_b_to_l);

			const float3 tr = rayleigh.CalculateTransmittance(ha, angle_a_to_b, hb, angle_b_to_l);
			const float3 tm = mie.CalculateTransmittance(ha, angle_a_to_b, hb, angle_b_to_l);

			// Calculate the sample contribution and sum. 

			const float3 sample = CalculateDensity(hb) * Exp(-(tr + tm)) * ds;

			ASSERT(!ContainsNan(sample) && !ContainsInf(sample));

			inscattering[x] += sample * beta / (4 * pi);
		}
	}
}

float3 Medium::CalculateTransmittance(float ha, float angle_a_to_b, float hb, float angle_b_to_l) const
{
	// Calculate the optical depth along the path from point a to point b in direction cos_angle_to_b
	// from a, then to point c at the edge of the atmosphere in the direction of the light. In order
	// to calculate this, we also need to consider point d at the edge of the atmosphere in the direction
	// from cos_angle_to_b from a.
	//
	//
	//            c         d
	//             \       /
	//              \     /
	//               \   /
	//                \ /
	//                 b
	//                /
	//               /
	//              a
	//
	// --------------------------------------------
	//
	// We can use the precomputed optical depth look up table t(h, theta), but we need to consider that the
	// angle is the angle from the local zenith. This means that the angle to the light from a will be
	// different to the angle to the light from b.
	//
	// depth(a, b) = lut(ha, angle_a_to_b) - lut(hb, angle_a_to_b)
	// depth(b, c) = lut(hb, angle_b_to_l)

	const float ad = depth.Sample(ha, angle_a_to_b);
	const float bd = depth.Sample(hb, angle_a_to_b);
	const float bc = depth.Sample(hb, angle_b_to_l);

	const float abc = ad - bd + bc;

	return beta * abc;
}

float Medium::CalculateDensity(float height) const
{
	return Exp(-Max(0.0f, height) / density_height);
}
