#pragma once

#include <Core/Types.h>

namespace MortonCode
{
	// https://fgiesen.wordpress.com/2009/12/13/decoding-morton-codes/

	inline uint32_t Part1By1(uint32_t x)
	{
		x &= 0x0000ffff;                 // x = ---- ---- ---- ---- fedc ba98 7654 3210
		x = (x ^ (x << 8)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
		x = (x ^ (x << 4)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
		x = (x ^ (x << 2)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
		x = (x ^ (x << 1)) & 0x55555555; // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
		return x;
	}

	inline int Encode(int x, int y)
	{
		return (Part1By1(y) << 1) | Part1By1(x);
	}

	inline uint32_t Compact1By1(uint32_t x)
	{
		x &= 0x55555555;                 // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
		x = (x ^ (x >> 1)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
		x = (x ^ (x >> 2)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
		x = (x ^ (x >> 4)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
		x = (x ^ (x >> 8)) & 0x0000ffff; // x = ---- ---- ---- ---- fedc ba98 7654 3210
		return x;
	}

	inline void Decode(int& x, int& y, int code)
	{
		x = Compact1By1(code >> 0);
		y = Compact1By1(code >> 1);
	}
}



