#pragma once

#include <RayTracer/Shape.h>
#include <RayTracer/Material.h>
#include <Math/Vector.h>

struct Intersection;
struct Material;

struct PlaneShape : Shape
{
	PlaneShape(float3 point, float3 normal, const Material& material) : point(point), normal(normal), material(material) {}

	// Return true if the intersection can be improved
	//
	bool Hit(float& tbest, const Ray& ray) const override;

	// Populate the intersection details
	//
	void PopulateIntersection(Intersection& intersection, float t, const Ray& ray) const override;

	// World-space center
	//
	float3 point = { 0, 0, 0 };

	// World-space normal
	//
	float3 normal = { 0, 1, 0 };

	// Material info
	//
	Material material;
};
