#include <RayTracer/Brdf/Lambert.h>
#include <Math/Matrix.h>
#include <Core/Constants.h>

float3 Lambert::CalculateLightDirection(float3 n, float2 sample)
{
	// p(ω) is proportional to cos(θ), so find the normalization constant to ensure the pdf integrates to 1:
	//
	// c ∫{0, 2π, cos(θ) dω} = 1                 (1)
	//
	// Transform to integral over polar coordinates remembering:
	//
	// dω = sin(θ) dθ dφ
	//
	// and the double angle trignometric identity:
	//
	// 2 cos(θ) sin(θ) = sin(2θ)
	//
	// ∫{0, 2π, cos(θ) dω} = ∫{0, 2π, ∫{0, π/2, cos(θ) sin(θ) dθ} dφ}
	//                     = ∫{0, 2π, ∫{0, π/2, sin(2θ) / 2 dθ} dφ}
	//                     = (1 / 2) ∫{0, 2π, ∫{0, π/2, sin(2θ) dθ} dφ}
	//                     = (1 / 2) ∫{0, 2π, [0, π/2, -cos(2θ) / 2] dφ}
	//                     = (1 / 2) ∫{0, 2π, (-cos(π) / 2) - (-cos(0) / 2) dφ}
	//                     = (1 / 2) ∫{0, 2π, 1 dφ}
	//                     = 2π / 2
	//                     = π
	//
	// Substitute the result of the integral back into (1):
	//
	// c ∫{0, 2π, cos(θ) dω} = cπ = 1
	//
	// So c = 1 / π
	//
	// Therefore the final pdf is:
	//
	// p(ω) = cos(θ) / π
	//
	// But we can't work using solid angles, we have to use polar coordinates instead. We know
	// that we can express the part inside the pdf integral equivalently as:
	//
	// p(θ, φ) dθ dφ = p(ω) dω
	//
	// We also know:
	//
	// dω = sin(θ) dθ dφ
	//
	// Therefore:
	//
	// p(θ, φ) dθ dφ = p(ω) sin(θ) dθ dφ
	// p(θ, φ) = p(ω) sin(θ)
	//         = cos(θ) sin(θ) / π
	//
	// We can now calculate the pdf for θ by integrating the joint pdf over φ:
	//
	// p(θ) = ∫{0, 2π, p(θ, φ) dφ}
	//      = ∫{0, 2π, cos(θ) sin(θ) / π dφ}
	//      = (cos(θ) sin(θ) / π) ∫{0, 2π, 1 dφ}
	//      = (cos(θ) sin(θ) / π) 2π
	//      = 2 cos(θ) sin(θ)
	//      = sin(2θ)
	//
	// And calculate the pdf for φ:
	//
	// p(φ) = p(θ, φ) / p(θ)
	//      = cos(θ) sin(θ) / (π sin(2θ))
	//      = sin(2θ) / (2π sin(2θ))
	//      = 1 / 2π
	//
	// Now that we have separate pdfs for our polar variables, we can calculate their cdfs by
	// integrating them:
	//
	// c(θ) = ∫{0, θ, p(θ) dθ}
	//      = ∫{0, θ, sin(2θ) dθ}
	//      = [0, θ, -cos(2θ) / 2]
	//      = (1 / 2) (-cos(2θ) - (-cos(0))
	//      = (1 / 2) (-cos(2θ) + 1)
	//      = (1 - cos(2θ)) / 2
	//
	// Note the double angle formula:
	//
	// cos(2θ) = 2 cos²(θ) - 1
	//
	// So we can simplify further:
	//
	// c(θ) = (1 - cos(2θ)) / 2
	//      = (1 - (2 cos²(θ) - 1)) / 2
	//      = (1 - 2 cos²(θ) + 1) / 2
	//      = (2 - 2 cos²(θ)) / 2
	//      = 1 - cos²(θ)
	//
	// c(φ) = ∫{0, φ, p(φ) dφ}
	//      = ∫{0, φ, 1 / 2π dφ}
	//      = (1 / 2π) ∫{0, φ, 1 dφ}
	//      = φ / 2π
	//
	// Now that we have the cdfs for both variables, we can invert them to solve for their
	// angles given a uniform probablilty:
	//
	// u = 1 - cos²(θ)
	// θ = acos(Sqrt(1 - u))
	//
	// v = φ / 2π
	// φ = 2πv
	//
	// Note that since u is a uniform random variable in the range [0, 1], the probability of picking
	// u is exactly the same as the probability of picking 1 - u, so we can rewrite the inverted cdf for
	// θ as follows:
	//
	// θ = acos(Sqrt(u))
	//
	// So clearly, u = cos²(θ)
	//
	// We can now pick θ and φ from uniform random varaibles u and v, but we need to convert from polar
	// form to cartesian form:
	//
	// x = sin(θ) * cos(φ)
	// y = cos(θ)
	// z = sin(θ) * sin(φ)
	//
	// Note that since:
	//
	// cos²(θ) + sin²(θ) = 1
	//
	// sin(θ) = Sqrt(1 - cos²(θ))
	//        = Sqrt(1 - u)

	const float phi = 2 * pi * sample.y;

	const float cos_theta = Sqrt(sample.x);
	const float sin_theta = Sqrt(1 - sample.x);

	return MakeRotationMatrix2(n) * float3(sin_theta * Cos(phi), sin_theta * Sin(phi), cos_theta);
}
