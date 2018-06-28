#pragma once

#include <Core/Types.h>

namespace Stats
{
	// Notify that a render will start
	//
	void OnStartRender(int w, int h, int quality);

	// Notify that a render has completed
	//
	void OnFinishRender();

	// Start and finish render times
	//
	extern uint64_t Start;
	extern uint64_t Finish;

	// Resolution last render
	//
	extern int Width;
	extern int Height;

	// Quality level used at the last render
	//
	extern int Quality;

	// Number of rays cast in total
	//
	extern uint64_t TotalRays;

	// Number of rays cast on a particular thread
	//
	extern thread_local uint64_t Rays;

	// Log all stats after a run has completed
	//
	void Log();
}
