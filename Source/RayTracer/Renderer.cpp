#include <RayTracer/Renderer.h>
#include <RayTracer/Integrator/Integrator.h>
#include <RayTracer/Brdf/FireflyReduction.h>
#include <RayTracer/Sampler.h>
#include <RayTracer/Camera.h>
#include <RayTracer/Tile.h>
#include <RayTracer/Scene.h>
#include <Image/Texel.h>
#include <System/Window.h>
#include <Core/Memory.h>

namespace
{
	float3 ToneMap(float3 x)
	{
		const float3 a = float3(2.51f);
		const float3 b = float3(0.03f);
		const float3 c = float3(2.43f);
		const float3 d = float3(0.59f);
		const float3 e = float3(0.14f);

		return Saturate((x*(a*x + b)) / (x*(c*x + d) + e));
	}

	//float3 ToneMap(float3 x)
	//{
	//	return Saturate(x);
	//}

	Bgra TransformToDisplaySpace(float3 x)
	{
		const float3 gamma = LinearToGamma(x);

		return { uint8_t(gamma.b * 255), uint8_t(gamma.g * 255), uint8_t(gamma.r * 255) };
	}
}

void Renderer::Render(Window& window, const Camera& camera, const Scene& scene, const Integrator& integrator) const
{
	Stats::OnStartRender(window.w, window.h, quality);

	MortonTiler tiler(Memory::TempAllocator(), window.w, window.h, 16);

	#pragma omp parallel for schedule(dynamic)

	for (int i = 0; i < tiler.count; ++i)
		RenderTile(window, tiler.GenerateTile(i), camera, scene, integrator);

	Stats::OnFinishRender();
	Stats::Log();
}

void Renderer::RenderTile(Window& window, Tile tile, const Camera& camera, const Scene& scene, const Integrator& integrator) const
{
	// We're going to render one tw x th tile into the ww x wh window  at the origin (ox, oy) 
	// determined by the tile index.

	const int ww = window.w;
	const int wh = window.h;

	// Fill the tile with orange and blit to the window while we're working on it

	Bgra texels[4096];

	ASSERT(tile.w * tile.h < sizeof(texels));

	for (int i = 0; i < tile.w * tile.h; ++i)
		texels[i] = { 30, 192, 255 };

	window.Blit(texels, tile.x, tile.y, tile.w, tile.h);

	// Sum up all AA samples from the path tracer to get final radiance values and then
	// tone map, convert to gamma space and quantize to window format.

	CorrelatedMultiJitterSampler sampler(quality);

	const int samples = sampler.GetSampleCount();
	const float dw = 1.0f / samples;

	for (int y = 0; y < tile.h; ++y)
	{
		for (int x = 0; x < tile.w; ++x)
		{
			const int px = tile.x + x;
			const int py = tile.y + y;

			sampler.StartPixel(px, py);

			Random::SetSeed(sampler.pattern);

			float3 radiance[2] = { { 0, 0, 0 }, { 0, 0, 0 } };

			for (int s = 0; s < samples; ++s)
			{
				FireflyReduction::RegisterNewSample();

				sampler.StartSample();

				const float2 sample = sampler.Get();

				const float u = (px + sample.x) * 2.0f / ww - 1.0f;
				const float v = (py + sample.y) * 2.0f / wh - 1.0f;

				radiance[s & 1] += integrator.Li(sampler, camera.GenerateRay(u, v, sampler.Get()), scene) * dw;
			}

			// Calculate error metric
			//
			// https://jo.dreggn.org/home/2009_stopping.pdf

			const float3 a = radiance[0] * camera.exposure;
			const float3 b = radiance[1] * camera.exposure;
			const float3 c = a + b;
			const float3 d = Abs(b - a);

			const float error = CalculateLuminance(d) / CalculateLuminance(c);

			texels[y * tile.w + x] = TransformToDisplaySpace(ToneMap(c));
		}
	}

	// Finally, blit the completed tile to the correct part of the window

	window.Blit(texels, tile.x, tile.y, tile.w, tile.h);
}
