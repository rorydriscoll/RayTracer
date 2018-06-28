#include <System/Time.h>
#include <System/Windows.h>
#include <Core/Types.h>

namespace
{
	uint64_t QueryFrequency()
	{
		LARGE_INTEGER counter_frequency;
		QueryPerformanceFrequency(&counter_frequency);

		return counter_frequency.QuadPart;
	}

	uint64_t QueryTicks()
	{
		LARGE_INTEGER counter_ticks;
		QueryPerformanceCounter(&counter_ticks);

		return counter_ticks.QuadPart;
	}

	float SystemTicksToSeconds(uint64_t ticks)
	{
		static uint64_t system_ticks_per_second = QueryFrequency();

		return float(double(ticks) / system_ticks_per_second);
	}
}

uint64_t Time::Now()
{
	return QueryTicks();
}

float Time::Elapsed(uint64_t begin, uint64_t end)
{
	return SystemTicksToSeconds(end - begin);
}
