#include <RayTracer/Integrator/DirectIntegrator.h>
#include <RayTracer/Material.h>
#include <RayTracer/Sampler.h>
#include <RayTracer/Intersection.h>
#include <RayTracer/Scene.h>
#include <Math/Ray.h>

float3 DirectIntegrator::Li(Sampler& sampler, Ray ray, const Scene& scene) const
{
	unused(sampler);

	Intersection intersection;

	if (!scene.Hit(intersection, ray))
		return scene.SampleEnvironment(ray);

	const float3 p = intersection.point;
	const float3 v = -ray.d;

	const UberBrdf brdf = intersection.material->CreateBrdf(intersection.uv, intersection.CalculateTransform());

	float3 color = { 0, 0, 0 };

	for (const auto& light : scene.lights)
	{
		const float3 l = light.direction;

		if (scene.Hit(Ray(p, l)))
			continue;

		const float3 li = light.irradiance;
		const float3 weight = brdf.Evaluate(l, v);

		color += li * weight;
	}

	return color;
}
