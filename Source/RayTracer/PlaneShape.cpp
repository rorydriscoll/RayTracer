#include <RayTracer/PlaneShape.h>
#include <RayTracer/Intersection.h>
#include <Math/Ray.h>

bool PlaneShape::Hit(float& tbest, const Ray& ray) const
{
	// Point on ray p + t * d hits the plane with center c and normal n when:
	//
	// ((p + t * d) - c).n = 0
	//
	// Distribute the dot product:
	//
	// (p - c).n + t * d.n = 0
	//
	// Simplify the relative offset q = p - c
	//
	// q.n + t * d.n = 0
	//
	// Solve for t
	//
	// t = -q.n / d.n
	//
	// In practice, we'll define q as c - p so we can skip the unary minus

	const float3 q = point - ray.p;
	const float3 d = ray.d;
	const float3 n = normal;

	const float qn = Dot(q, n);
	const float dn = Dot(d, n);

	if (dn == 0)
		return false;

	const float t = qn / dn;

	const float tmin = 0.00001f;
	const float tmax = tbest;

	if (t <= tmin)
		return false;

	if (t >= tmax)
		return false;

	tbest = t;

	return true;
}

void PlaneShape::PopulateIntersection(Intersection& intersection, float t, const Ray& ray) const
{
	const float3 p = ray.p + ray.d * t;

	intersection.t = t;
	intersection.point = p;
	intersection.normal = normal;
	intersection.uv = { p.x, p.z }; // Naughty - doesn't work properly for upright planes!
	intersection.dpdu = { 1, 0, 0 }; // Also naughty!
	intersection.dpdv = { 0, 0, 1 }; // FIXMESTEVE
	intersection.material = &material;
}
