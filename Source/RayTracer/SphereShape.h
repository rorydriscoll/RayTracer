#pragma once

#include <RayTracer/Shape.h>
#include <RayTracer/Material.h>
#include <Math/Vector.h>

struct SphereShape : Shape
{
	SphereShape(float3 center, float radius, const Material& material) : center(center), radius(radius), material(material) {}

	// Return true if the intersection can be improved
	//
	bool Hit(float& tbest, const Ray& ray) const override;

	// Populate the intersection details
	//
	void PopulateIntersection(Intersection& intersection, float t, const Ray& ray) const override;

	// World-space center
	//
	float3 center = { 0, 0, 0 };

	// World-space radius (m)
	//
	float radius = 1;

	// Material info
	//
	Material material;
};

