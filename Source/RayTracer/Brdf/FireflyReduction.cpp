#include <RayTracer/Brdf/FireflyReduction.h>
#include <Core/Generic.h>

namespace
{
	// The minimum allowed roughness for the current sample
	//
	thread_local float min_alpha = 0;

	// The roughness value at which firefly reduction will disable. Roughness values beyond about
	// GGX alpha = 0.25 don't tend to produce fireflies at decent sample rates. We want this to be
	// as low as possible since it will still allow some caustics, and will generate proper colored
	// reflections.
	//
	thread_local float max_min_alpha = 0.25f;
}

#define FIREFLY_REDUCTION

void FireflyReduction::RegisterNewSample()
{
	min_alpha = 0;
}

void FireflyReduction::RegisterBounce(float alpha)
{
	min_alpha = Max(min_alpha, Min(alpha, max_min_alpha));
}

float FireflyReduction::GetRoughness(float alpha)
{
#ifdef FIREFLY_REDUCTION
	return Max(alpha, min_alpha);
#else
	return alpha;
#endif
}
