#include <RayTracer/Brdf/Microfacet.h>
#include <Math/Matrix.h>
#include <Core/Generic.h>

float3 Microfacet::CalculateNormal(float3 v, float3 n, float2 sample, float alpha)
{
	const float3x3 world_from_geometry = MakeRotationMatrix2(n);
	const float3x3 geometry_from_world = Transpose(world_from_geometry);

	// https://hal.archives-ouvertes.fr/hal-01509746/document
	//
	// See the paper for a good diagram of what is going on here. We basically have two
	// half-disks and we're going to generate a point on one of them and then project it 
	// up onto the hemisphere above.
	//
	// The blue half-disk is aligned perpendicular to the view direction v (i.e. the disk
	// normal is v), and the green disk is aligned to the geometry surface (e.g. the disk
	// normal is n).
	//
	// Note that we will be projecting onto the hemisphere above the blue disk, so we will
	// project in the view direction.
	//
	// When choosing a random point on either of the disks, we have to account for the 
	// projected area of the green disk onto the blue disk to ensure we get a proper 
	// distribution of points.
	//
	// Note that this method takes advantage of a very interesting property that we can
	// stretch the vectors (and renormalize) into unit-roughness space, solve for the
	// output, and then compress back into the actual roughness space. This makes the
	// whole thing tractable since alpha is always one in the unit roughness space, so the
	// shape of the pdf is just a hemisphere.
	//
	// Note that for disk sampling, we just need polar coordinates, being careful to ensure
	// that the angle is in the right part of the disk, and that the radius accounts for the
	// increasing area (i.e. we use the square root of the random variable).

	const float ra = sample.x;
	const float rb = sample.y;

	// Transform the view vector from world space into geometry space, stretch in geometry space 
	// and renormalize to account for the roughness.

	const float3 vw = v;
	const float3 vg = geometry_from_world * vw;
	const float3 vs = Normalize(float3(alpha * vg.x, alpha * vg.y, vg.z));

	// The projected area of the the green disk is proportional to v.n, and the projected area
	// of the blue disk is 1, so we can calculate a threshold value to determine which disk to
	// sample on using v.

	const float area_blue = 1;
	const float area_green = vs.z;

	const float prob_blue = 1 / (area_blue + area_green);
	const float prob_green = 1 - prob_blue;

	// Calculate the polar coordinates of the point based on the disk that was selected by v. This
	// point lies on the disk that was selected.

	const float phi = (rb < prob_blue) ? rb / prob_blue * pi : pi + (rb - prob_blue) / prob_green * pi;
	const float r = Sqrt(ra);

	// Project the point onto the blue disk, then project up to the hemisphere above the blue disk.
	// This puts the point on the hemisphere of the geometry too.

	const float x = r * Cos(phi);
	const float y = r * Sin(phi) * (rb < prob_blue ? area_blue : area_green);
	const float z = Sqrt(Max(0.0f, 1.0f - x * x - y * y));

	// Transform from blue-disk space into geometry space, unstretch in geometry-space, transform
	// into world-space, then normalize.

	const float3 ms = MakeRotationMatrix2(vs) * float3(x, y, z);
	const float3 mg = float3(alpha * ms.x, alpha * ms.y, Max(0.0f, ms.z));
	
	return Normalize(world_from_geometry * mg);
}
