#include <RayTracer/Brdf/LambertBrdf.h>
#include <RayTracer/Brdf/Lambert.h>
#include <RayTracer/Brdf/FireflyReduction.h>
#include <Math/Matrix.h>

float3 LambertBrdf::Evaluate(float3 l, float3 v) const
{
	unused(v);

	return Lambert::Evaluate(Saturate(Dot(normal, l)), albedo);
}

float3 LambertBrdf::Sample(float3& l, float3 v, float2 sample) const
{
	unused(v);

	// Register a rough bounce with the microfacet model for firefly reduction

	FireflyReduction::RegisterBounce(1.0f);

	// Calculate the bounce direction

	l = Lambert::CalculateLightDirection(normal, sample);

	// The brdf is:
	//
	// f(θ, φ) = cos(θ) / π
	//
	// And the pdf is:
	//
	// p(θ, φ) = cos(θ) / π
	//
	// So the pdf-weighted brdf is:
	//
	// w(θ, φ) = f(θ, φ) / p(θ, φ) 
	//         = 1
	//
	// We'll multiply through by the albedo to get the final weight

	return albedo;
}
