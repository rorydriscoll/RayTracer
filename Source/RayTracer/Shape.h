#pragma once

struct Ray;
struct Intersection;

struct Shape
{
	virtual ~Shape() = default;

	// Return true if the intersection can be improved
	//
	virtual bool Hit(float& tbest, const Ray& ray) const = 0;

	// Populate the intersection details
	//
	virtual void PopulateIntersection(Intersection& intersection, float t, const Ray& ray) const = 0;
};
