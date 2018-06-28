#include <RayTracer/Integrator/DepthIntegrator.h>
#include <RayTracer/Sampler.h>
#include <RayTracer/Intersection.h>
#include <RayTracer/Scene.h>
#include <Math/Ray.h>

float3 DepthIntegrator::Li(Sampler& sampler, Ray ray, const Scene& scene) const
{
	unused(sampler);

	Intersection intersection;

	if (!scene.Hit(intersection, ray))
		return float3(1);

	return float3(intersection.t * depth_scale);
}
