#include <RayTracer/Integrator/PathIntegrator.h>
#include <RayTracer/Material.h>
#include <RayTracer/Sampler.h>
#include <RayTracer/Intersection.h>
#include <RayTracer/Scene.h>
#include <Math/Ray.h>

float3 PathIntegrator::Li(Sampler& sampler, Ray ray, const Scene& scene) const
{
	float3 color = { 0, 0, 0 };
	float3 coefficient = { 1, 1, 1 };

	for (int i = 0; i < max_depth; ++i)
	{
		Intersection intersection;

		if (!scene.Hit(intersection, ray))
			return color + scene.SampleEnvironment(ray) * coefficient;

		const float3 p = intersection.point;
		const float3 v = -ray.d;

		const UberBrdf brdf = intersection.material->CreateBrdf(intersection.uv, intersection.CalculateTransform());

		// Evaluate direct lighting

		for (const auto& light : scene.lights)
		{
			const float3 l = light.direction;

			if (scene.Hit(Ray(p, l)))
				continue;

			const float3 li = light.irradiance;
			const float3 weight = brdf.Evaluate(l, v);

			ASSERT(!ContainsNan(li));
			ASSERT(!ContainsNan(weight));

			color += li * weight * coefficient;
		}

		// Evaluate the material for the future interactions

		float3 l;
		const float3 weight = brdf.Sample(l, v, sampler.Get());

		ASSERT(!ContainsNan(weight));

		coefficient *= weight;

		// Set up the ray for the next bounce

		ray.p = p;
		ray.d = l;
	}

	return color;
}
