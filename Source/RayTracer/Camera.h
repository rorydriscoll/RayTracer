#pragma once

#include <Math/Ray.h>
#include <Core/Constants.h>

enum class Lens
{
	FL35mm   = 35,
	FL50mm   = 50,
	FL200mm  = 200,
};

enum class FStop
{
	F1     = 0,
	F1_4   = 1,
	F2     = 2,
	F2_8   = 3,
	F4     = 4,
	F5_6   = 5,
	F8     = 6,
	F11    = 7,
	F16    = 8,
	F22    = 9,
	F32    = 10,
	F45    = 11,
	F64    = 12,
	F90    = 13,
	F128   = 14
};

enum class Shutter
{
	SS1      = 1,
	SS2      = 2,
	SS4      = 4,
	SS8      = 8,
	SS15     = 15,
	SS30     = 30,
	SS60     = 60,
	SS100    = 100,
	SS125    = 125,
	SS250    = 250,
	SS500    = 500,
	SS1000   = 1000,
	SS2000   = 2000,
	SS4000   = 4000
};

enum class Iso
{
	ISO50      = 50,
	ISO100     = 100,
	ISO200     = 200,
	ISO400     = 400,
	ISO800     = 800,
	ISO1600    = 1600,
	ISO3200    = 3200,
	ISO6400    = 6400,
	ISO12800   = 12800,
	ISO25600   = 25600,
};

struct Camera
{
	Camera() = default;

	Camera(float3 position, float3 target) :
		position(position), focal_distance(Length(target - position))
	{
		z = Normalize(target - position);
		x = Normalize(Cross(float3(0, 1, 0), z));
		y = Cross(z, x);
	}

	void ApplySettings(float f, float N, float t, float S)
	{
		// https://placeholderart.wordpress.com/2014/11/21/implementing-a-physically-based-camera-manual-exposure/
		//
		// This whole function uses variable names that match the way it is presented on wikipedia
		//
		// f = focal length (m)
		// N = f-number
		// t = shutter speed (s)
		// S = ISO

		// Assuming a full frame DSLR sensor, so 36 mm x 24 mm

		const float camera_sensor_w = 36_mm;
		const float camera_sensor_h = 24_mm;

		// Scale the camera plate dimensions to remove the effect of the focal length. We're going
		// to store the half dimensions since that's what we need for generating the camera rays.

		hw = 0.5f * camera_sensor_w / f;
		hh = 0.5f * camera_sensor_h / f;

		// https://en.wikipedia.org/wiki/F-number
		//
		// The aperture diameter (D), focal length (f) and f-number (N) are related as follows:
		//
		// N = f / D
		//
		// We need to store the aperture radius, which is:
		//
		// r = f / (2 * N)
		//

		aperture = f / (2 * N);

		// https://en.wikipedia.org/wiki/Exposure_value
		// https://en.wikipedia.org/wiki/Film_speed
		//
		// We can calculate the proper exposure given all the camera settings. Start by looking 
		// at how the exposure value (EV) relates to the f-number (N) and the shutter speed (t)
		//
		// EV = log₂(N² / t)
		// 
		// I'm going to use Standard Output Sensitivity rather than Saturation Based Speed for
		// calculating the expsure. SBS works out to be a little bit darker than SOS and it seems
		// like things are too dark in general then.
		//
		// For SOS, the equation we want to solve is:
		//
		// S = 10 / H
		//
		// Where S is the ISO, and H is the lumninous exposure:
		//
		// H = q * L * t / N²
		//
		// L is the scene luminance, and q is a lens factor, typically around 0.65. We can solve 
		// this for luminance which will give us our *average* luminance. Typically, we then set
		// the exposure so that this average luminance maps to 18% grey (linear)
		//
		// e = 0.18 / L
		//
		// Let's substitute in the value of H:
		//
		// S = 10 / (q * L * t / N²)
		//
		// Rearrange and solve for L
		//
		// L = 10 / (q * S * t / N²)
		//   = (N² / t) * 10 / (q * S)
		//   = pow(2, EV) * 10 / (q * S)
		//
		// Assuming that q is the typical lens factor, we end up with
		//
		// L = pow(2, EV) * 1000 / (65 * S)
		//
		// Substitute in the definition of EV
		//
		// L = (1000 / 65) * N² / (S * t)
		//
		// Now substitute back into the exposure calculation:
		//
		// e = 0.18 / L
		// e = (18 / 100) * (1 / L)
		//   = (18 / 100) * 1 / ((1000 / 65) * N² / (S * t))
		//   = (18 / 100) * (65 / 1000) * S * t / N²
		//   = (117 / 10000) * S * t / N²

		exposure = 0.0117f * S * t / (N * N);
	}

	void ApplySettings(Lens lens, FStop fstop, Shutter shutter, Iso iso)
	{
		// Transform to values we can use to apply the settings. See the numeric version of this
		// function for lots more details.
		//
		// Note that the f-number (N) is related to the f-stop exponent (r):
		//
		// N = pow(sqrt(2), r)

		const float f = float(lens) * 0.001f;
		const float N = Pow(sqrt2, float(fstop));
		const float t = 1.0f / float(shutter);
		const float S = float(iso);

		ApplySettings(f, N, t, S);
	}

	Ray GenerateRay(float u, float v, float2 sample) const
	{
		// Use the normalized pixel position (u, v) in the range [-1, +1] where (0, 0) is the 
		// center of the screen, and the camera near clip plane (assumed to be at 1 meter from 
		// the camera with dimensions (w, h)) to genrate the ray aim point, then normalize to 
		// get the pinhold camera direction.
		//
		// <- 1 ->
		//      .'
		//    .' |
		//  .'   |
		// *-._  |
		//  '. '-o
		//    '. |
		//      '.

		const float3 direction = Normalize(u * hw * x - v * hh * y + z);

		// For depth of field, we'll look at where the pinhole ray intersects the focal plane
		// at distance fd from the camera. This will be our focus aim point.
		//
		// <---- fd ---->
		//
		//              .
		//            .'|
		//          .'  |
		//        .'    |
		//      .'      |
		//    .'|       |
		//  .'  |       |
		// *-._ |       |
		//  '. '-._     |
		//    '.|  '-._ |
		//      '.     'o
		//        '.    |
		//          '.  |
		//            '.|
		//              '

		const float3 focus = position + direction * (focal_distance / Dot(direction, z));

		// We'll then jitter the camera position based on the lens radius r and a random sample. The 
		// lens radius effectively dictates how deep/shallow the depth of field effect will be. Once 
		// we have the jittered camera position, we'll point the camera ray back at the original focal 
		// point.
		//
		//              .
		//            .'|
		//          .'  |
		//        .'    |
		// *.   .'      |
		//   '.'|       |
		//  .' '.       |
		// *    |'.     |
		//  '.  |  '.   |
		//    '.|    '. |
		//      '.     'o
		//        '.    |
		//          '.  |
		//            '.|
		//              '

		const float theta = sample.x * 2 * pi;
		const float r = Sqrt(sample.y) * aperture;

		const float dx = r * Cos(theta);
		const float dy = r * Sin(theta);

		const float3 origin = position + x * dx + y * dy;

		return { origin, Normalize(focus - origin) };
	}

	// World-space position
	//
	float3 position = { 0, 0, -5 };

	// World-space axes
	//
	float3 x = { 1, 0, 0 };
	float3 y = { 0, 1, 0 };
	float3 z = { 0, 0, 1 };

	// Camera plate dimensions (m)
	//
	float hh = 1;
	float hw = 2;

	// Focal distance (m)
	//
	float focal_distance = 1;

	// Aperture (actually the radius of the camera lens) (m)
	//
	float aperture = 0.02f;

	// Exposure value
	//
	float exposure = 1;
};

