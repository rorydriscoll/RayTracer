#include <RayTracer/Brdf/UberBrdf.h>
#include <RayTracer/Brdf/Microfacet.h>
#include <RayTracer/Brdf/Lambert.h>
#include <RayTracer/Brdf/FireflyReduction.h>
#include <Math/Random.h>

float3 UberBrdf::Evaluate(float3 l, float3 v) const
{
	// This is a combination of a microfacet specular layer with a Lambert diffuse layer
	// at the bottom. For energy-conservation purposes, only the light that the specular
	// layer transmits can be used for layers below.
	//
	// This can be extended to multiple specular layers pretty easily, but I don't need
	// to do so at the moment.

	const float3 n = normal;
	const float3 h = Normalize(l + v, n);

	const float vdoth = Saturate(Dot(v, h));
	const float ndotl = Saturate(Dot(n, l));
	const float ndotv = Saturate(Dot(n, v));
	const float ndoth = Saturate(Dot(n, h));

	const float3 reflected = Microfacet::F(reflectance, vdoth);
	const float3 transmitted = 1.0f - reflected;

	return transmitted * Lambert::Evaluate(ndotl, albedo) + reflected * Microfacet::V(alpha, ndotl, ndotv) * Microfacet::D(alpha, ndoth) * ndotl;
}

float3 UberBrdf::Sample(float3& l, float3 v, float2 sample) const
{
	// This is a combination of a microfacet specular layer with a Lambert diffuse layer
	// at the bottom. For energy-conservation purposes, only the light that the specular
	// layer transmits can be used for layers below.
	//
	// We will use russian roulette to decide which layer will reflect the current ray
	// by using the amount of light reflected from the Fresnel function. Note that since
	// we're using a random variable to choose between layers, we don't need to multiply
	// through by the fresnel reflectance. We still need to factor in the specular and
	// diffuse tints though, in particular remembering to tint the diffuse factor by the
	// *transmitted* specular tint.
	
	const float3 n = normal;
	const float3 h = Microfacet::CalculateNormal(v, n, sample, alpha);
	
	const float3 fresnel = Microfacet::F(reflectance, Saturate(Dot(v, h)));
	
	const float ps = Length(fresnel);
	const float pd = Length(albedo);

	const float pdf = ps / (ps + pd);

	if (Random::Real() < pdf)
	{
		// Tell the firefly reduction system that we're doing a specular bounce

		FireflyReduction::RegisterBounce(alpha);

		// Reflect the view vector about the microfacet normal to get the light vector

		l = Reflect(-v, h);

		// Return the sample weight (see the MicrofacetBrdf for the derivation of this)
		// Note that due to the russian roulette, we're dividing by the pdf which is f
		// in this case. That totally cancels out the fresnel term.

		return fresnel / pdf * Microfacet::G1(alpha, Saturate(Dot(n, l)));
	}
	else
	{
		// Tell the firefly reduction system that we're doing a diffuse bounce

		FireflyReduction::RegisterBounce(1.0f);

		// Calculate the light vector based off the random sample

		l = Lambert::CalculateLightDirection(n, sample);

		// Return the sample weight (see the LambertBrdf for the derivation of this).
		// As in the specular case, we're dividing by the transmission probability here
		// which is (1 - f). We also have to take care of the fact that the light will
		// be attenuated by the specular color.

		return (1 - fresnel) / (1 - pdf) * albedo;
	}
}
