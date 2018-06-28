#pragma once

#include <Core/Types.h>

namespace Time
{
	// Current time in system ticks
	//
	uint64_t Now();

	// Calculate the elapsed time in seconds between two times
	//
	float Elapsed(uint64_t begin, uint64_t end);
}