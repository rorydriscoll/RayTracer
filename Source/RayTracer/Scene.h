#pragma once

#include <RayTracer/Atmosphere.h>
#include <RayTracer/Intersection.h>
#include <RayTracer/SphereShape.h>
#include <RayTracer/PlaneShape.h>
#include <RayTracer/Light.h>
#include <RayTracer/Stats.h>
#include <Math/Ray.h>
#include <vector>

struct Scene
{
	Scene() : atmosphere(Normalize(float3({ 0, 1, 0 })), 1, 0.8f, 1)
	{
	}

	bool Hit(Intersection& intersection, const Ray& ray) const
	{
		++Stats::Rays;

		float t = max_float_value;

		const Shape* hit = nullptr;

		for (const auto& sphere : spheres)
		{
			if (sphere.Hit(t, ray))
				hit = &sphere;
		}

		for (const auto& plane : planes)
		{
			if (plane.Hit(t, ray))
				hit = &plane;
		}

		if (!hit)
			return false;

		hit->PopulateIntersection(intersection, t, ray);

		return true;
	}

	bool Hit(const Ray& ray) const
	{
		++Stats::Rays;

		float t = max_float_value;

		for (const auto& sphere : spheres)
		{
			if (sphere.Hit(t, ray))
				return true;
		}

		for (const auto& plane : planes)
		{
			if (plane.Hit(t, ray))
				return true;
		}

		return false;
	}

	float3 SampleEnvironment(const Ray& ray) const
	{
		//unused(ray);
		//return float3(8000);
		return atmosphere.CalculateInscattering(ray.d) * atmosphere.sun.irradiance;
	}

	std::vector<SphereShape> spheres;
	std::vector<PlaneShape> planes;
	std::vector<Light> lights;

	Atmosphere atmosphere;
};
