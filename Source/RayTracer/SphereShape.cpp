#include <RayTracer/SphereShape.h>
#include <RayTracer/Intersection.h>
#include <Math/Ray.h>

bool SphereShape::Hit(float& tbest, const Ray& ray) const
{
	// Point on ray p + t * d hits the sphere with center c and radius r when:
	//
	// || p + t * d - c || = r
	//
	// Square both sides:
	//
	// (p + t * d - c).(p + t * d - c) = r * r
	// 
	// Simplify the relative offset q = p - c
	//
	// (q + t * d).(q + t * d) = r * r
	//
	// Expand to quadratic form:
	//
	// q.q + 2 * q.d * t + d.d * t * t = r * r
	// d.d * t^2 + 2 * q.d * t + q.q - r * r = 0
	//
	// Quadratic solution, noting that d.d = 1 since the ray direction is normalized:
	//
	// A = 1
	// B = 2 * q.d
	// C = q.q - r * r
	//
	// t = (-B - Sqrt(B * B - 4 * A * C)) / (2 * A)
	//   = (-2 * q.d - Sqrt(2 * q.d * 2 * q.d - 4 * (q.q - r * r))) / 2
	//   = (-2 * q.d - Sqrt(4 * q.d * q.d - 4 * (q.q - r * r))) / 2
	//   = (-2 * q.d - Sqrt(4 * (q.d * q.d - (q.q - r * r))) / 2
	//   = (-2 * q.d - 2 * Sqrt(q.d * q.d - (q.q - r * r)) / 2
	//   = (-q.d - Sqrt(q.d * q.d - (q.q - r * r))
	//   = (-q.d - Sqrt(q.d * q.d - q.q + r * r)

	const float3 q = ray.p - center;
	const float3 d = ray.d;

	const float qd = Dot(q, d);
	const float qq = Dot(q, q);
	const float rr = radius * radius;

	const float discriminant = qd * qd - qq + rr;

	if (discriminant < 0)
		return false;

	const float t = -qd - Sqrt(discriminant);

	const float tmin = 0.00001f;
	const float tmax = tbest;

	if (t <= tmin)
		return false;

	if (t >= tmax)
		return false;

	tbest = t;

	return true;
}

void SphereShape::PopulateIntersection(Intersection& intersection, float t, const Ray& ray) const
{
	const float3 p = ray.p + ray.d * t;
	const float3 n = Normalize(p - center);

	// UV
	//
	// n.x = sin(theta) * cos(phi)
	// n.y = cos(theta)
	// n.z = sin(theta) * sin(phi)
	//
	// theta = acos(n.y);
	// phi = atan(n.z / n.x)
	//
	// u = theta / pi
	// v = phi / (2 pi)
	//
	// Derivatives
	//
	// dpdu - y value is unchanged, so just take perpendicular on the surface and scale
	// up to include the radius and 2 pi radians
	//
	// dpdu = { -n.z, 0, n.x } * radius * tau

	const float theta = ACos(n.y);
	const float phi = pi + ATan(n.z, n.x);

	const float cos_theta = n.y;
	const float sin_theta = Sqrt(Saturate(1.0f - cos_theta * cos_theta));

	const float sin_phi = sin_theta == 0 ? 0 : n.x / sin_theta;
	const float cos_phi = sin_theta == 0 ? 1 : n.z / sin_theta;

	intersection.t = t;
	intersection.point = p;
	intersection.normal = n;
	intersection.uv = { 2 * phi * invtau, theta * invpi }; // Temp adding x 2 on u so that it doesn't stretch the textures
	intersection.dpdu = float3(-n.z, 0, n.x) * radius * tau;
	intersection.dpdv = float3(cos_theta * sin_phi, -sin_theta, cos_theta * cos_phi) * radius * pi;
	intersection.material = &material;
}
