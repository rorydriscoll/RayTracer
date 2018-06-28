#include <RayTracer/Stats.h>
#include <System/Time.h>
#include <Core/Log.h>

uint64_t Stats::Start = 0;
uint64_t Stats::Finish = 0;
uint64_t Stats::TotalRays = 0;

int Stats::Quality = 0;
int Stats::Width = 0;
int Stats::Height = 0;

thread_local uint64_t Stats::Rays = 0;

void Stats::OnStartRender(int w, int h, int quality)
{
	#pragma omp parallel
	{
		Rays = 0;
	}

	Start = Time::Now();
	Finish = Start;
	Width = w;
	Height = h;
	Quality = quality;

	TotalRays = 0;
}

void Stats::OnFinishRender()
{
	Finish = Time::Now();

	#pragma omp parallel
	{
		#pragma omp atomic
		TotalRays += Rays;
	}
}

void Stats::Log()
{
	const float duration = Time::Elapsed(Start, Finish);

	LOG_INFO("Render (%i x %i): quality = %i, rays = %llu, duration = %.2f s, efficiency = %.2f Mray/s", Width, Height, Quality, TotalRays, duration, (TotalRays * 0.000001f) / duration);
}
