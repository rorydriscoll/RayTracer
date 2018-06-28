#pragma once

#include <Math/Scalar.h>
#include <Math/Vector.h>
#include <Core/Constants.h>

namespace Microfacet
{
	inline float3 F(float3 f0, float vdoth)
	{
		// Schlick Fresnel approximation
		//
		// F(v, h) = f₀ + (1 - f₀) * (1 - v·h)⁵

		return f0 + (float3(1) - f0) * Pow(1 - vdoth, 5);
	}

	inline float F(float f0, float vdoth)
	{
		// Schlick Fresnel approximation
		//
		// F(v, h) = f₀ + (1 - f₀) * (1 - v·h)⁵

		return f0 + (1 - f0) * Pow(1 - vdoth, 5);
	}

	inline float G1(float alpha, float ndotx)
	{
		// Smith GGX geometry factor for either shadowing or masking
		//
		// G₁(n, x) = (2 n·x) / (n·x + sqrt(α² + (1 - α²)(n·x)²))

		const float aa = alpha * alpha;

		return 2 * ndotx / (ndotx + Sqrt(aa + (1 - aa) * ndotx * ndotx));
	}

	inline float G(float alpha, float ndotl, float ndotv)
	{
		// Smith GGX geometry factor for both shadowing and masking
		//
		// G = G₁(n, l) G₁(n, v)

		return G1(alpha, ndotl) * G1(alpha, ndotv);
	}

	inline float V1(float alpha, float ndotx)
	{
		// Smith GGX visibility factor for either shadowing or masking
		//
		// V₁(n, x) = 1 / (n·x + sqrt(α² + (1 - α²)(n·x)²)

		const float aa = alpha * alpha;

		return 1 / (ndotx + Sqrt(aa + (1 - aa) * ndotx * ndotx));
	}

	inline float V(float alpha, float ndotl, float ndotv)
	{
		// Smith GGX visibility factor for both shadowing and masking
		//
		// V = V₁(n, l) V₁(n, v)

		return V1(alpha, ndotl) * V1(alpha, ndotv);
	}

	inline float D(float alpha, float ndoth)
	{
		// Smith GGX normal distribution
		//
		// D(h) = α² / (π ((α² - 1) (n·h)² + 1)²)

		const float aa = alpha * alpha;
		const float q = (aa - 1) * ndoth * ndoth + 1;

		return aa / (pi * q * q);
	}

	// Calculate the microfacet normal from the geometry normal, view vector and random sample
	//
	float3 CalculateNormal(float3 v, float3 n, float2 sample, float alpha);
}