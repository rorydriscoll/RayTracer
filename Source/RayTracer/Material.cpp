#include <RayTracer/Material.h>
#include <RayTracer/Brdf/UberBrdf.h>
#include <RayTracer/Intersection.h>
#include <Core/Generic.h>

UberBrdf Material::CreateBrdf(float2 uv, float3x3 world_from_surface) const
{
	const float metal = metalness->Sample(uv);
	
	const float3 kx = color->Sample(uv);
	const float3 kd = Lerp(kx, float3(0.00f), metal);
	const float3 ks = Lerp(float3(0.04f), kx, metal);
	const float3 nt = Normalize(normal->Sample(uv) * float3(2, 2, 1) - float3(1, 1, 0));

	return UberBrdf(world_from_surface * nt, kd, ks, roughness->Sample(uv));
}
