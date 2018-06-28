#include <RayTracer/Brdf/MicrofacetBrdf.h>
#include <RayTracer/Brdf/Microfacet.h>
#include <RayTracer/Brdf/FireflyReduction.h>

// References:
//
// Crafting a Next-Gen Brdf Pipeline for The Order : 1886
// http://blog.selfshadow.com/publications/s2013-shading-course/rad/s2013_pbs_rad_notes.pdf
//
// A Simpler and Exact Sampling Routine for the GGX Distribution of Visible Normals (Heitz)
// https://hal.archives-ouvertes.fr/hal-01509746/document
// 
// Importance Sampling techniques for GGX with Smith Masking-Shadowing: Part 2 (Joe Schutte)
// https://schuttejoe.github.io/post/ggximportancesamplingpart2/

float3 MicrofacetBrdf::Evaluate(float3 l, float3 v) const
{
	// Microfacet brdf based on Cook-Torrance
	//
	// v = ωo
	// l = ωi
	// h = normalize(l + v)
	//
	// f(l, v, n) = F(v, h) G(l, v, h) D(h) / (4 n·l n·v)
	//
	// For the Fresnel term, we'll use Schlicks approximation:
	//
	// F(v, h) = f₀ + (1 - f₀) * (1 - v·h)⁵
	//
	// For the visibility term, we have to match the normal distribution function for energy
	// conservation purposes, so we'll use the Smith non-correlated term:
	//
	// G(l, v, h) = G₁(n, l) G₁(n, v)
	//
	// Where:
	//
	// G₁(n, x) = (2 n·x) / (n·x + sqrt(1 + α²(1 - (n·x)²)))
	//
	// For the normal distribution term, I'm using GGX
	//
	// D(h) = α² / (π ((α² - 1) (n·h)² + 1)²)
	//
	// Looking at the terms on the top of the geometry factor, they cancel out with those
	// on the bottom, so we can use the visibility formulation from the RAD paper:
	//
	// f(l, v, n) = F(v, h) V(l, v, h) D(h)
	//
	// Multiply through by the cos theta term for the final sample weight:
	//
	// w(l, v, n) = f(l, v, n) n·l = F(v, h) V(l, v, h) D(h) n·l

	const float3 n = normal;
	const float3 h = Normalize(l + v, n);

	const float ndotl = Saturate(Dot(n, l));
	const float ndotv = Saturate(Dot(n, v));
	const float ndoth = Saturate(Dot(n, h));
	const float vdoth = Saturate(Dot(v, h));

	return Microfacet::F(reflectance, vdoth) * Microfacet::V(alpha, ndotl, ndotv) * Microfacet::D(alpha, ndoth) * ndotl;
}

float3 MicrofacetBrdf::Sample(float3& l, float3 v, float2 sample) const
{
	// Tell the firefly reduction that we're bouncing with our ideal roughness

	FireflyReduction::RegisterBounce(alpha);

	// Cook-Torrance brdf
	//
	// v = ωo
	// l = ωi
	// h = normalize(l + v)
	//
	// f(l, v, n) = F(v, h) G(l, v, h) D(h) / (4 n·l n·v)
	//
	// For the Fresnel term, we'll use Schlicks approximation:
	//
	// F(v, h) = f₀ + (1 - f₀) * (1 - v·h)⁵
	//
	// For the visibility term, we have to match the normal distribution function for energy
	// conservation purposes, so we'll use the Smith non-correlated term:
	//
	// G(l, v, n) = G₁(n, l) G₁(n, v)
	//
	// Where:
	//
	// G₁(n, x) = (2 n·x) / (n·x + sqrt(1 + α²(1 - (n·x)²)))
	//
	// PDF details: https://schuttejoe.github.io/post/ggximportancesamplingpart2/
	//
	// For the normal distribution term, I'm using GGX
	//
	// D(h) = α² / (π ((α² - 1) (n·h)² + 1)²)
	//
	// The pdf operates on the *visible* normals:
	//
	// Dᵥ(h) = G₁(v, n) D(h) v·h / n·v
	//
	// Since the gometry term normnalizes the ndf, the pdf is the same:
	//
	// pᵥ(h) = Dᵥ(h)
	// 
	// For the pdf, we have to account for the fact that we're transforming from the half-vector
	// space into the incident light space. This causes an extra factor to appear. See 'One Extra 
	// Step' in https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf
	//
	// p(h) = pᵥ(h) / (4 v·h)
	//      = G₁(v, n) D(h) v·h / (4 v·h n·v)
	//      = G₁(v, n) D(h) / (4 n·v)
	//
	// After adding the cos theta term from the integral, the final sampling weight is then:
	//
	// w(l, v, n) = F(v, h) G(l, v, h) D(h) n·l / (4 n·l n·v p(h))
	//
	// Expand out the definition for p(h) and start canceling out factors:
	//
	// w(l, v, n) = F(v, h) G(l, v, h) D(h) n·l / (4 n·l n·v p(h))
	//            = F(v, h) G(l, v, h) D(h) 4 n·v n·l / (4 n·l n·v G₁(v, n) D(h))
	//            = F(v, h) G(l, v, h) / G₁(v, n)
	//            = F(v, h) G₁(l, n)

	const float3 n = normal;
	const float3 h = Microfacet::CalculateNormal(v, n, sample, alpha);
	
	l = Reflect(-v, h);

	const float ndotl = Saturate(Dot(n, l));
	const float vdoth = Saturate(Dot(v, h));

	return Microfacet::F(reflectance, vdoth) * Microfacet::G1(alpha, ndotl);
}
