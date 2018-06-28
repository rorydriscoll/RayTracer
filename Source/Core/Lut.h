#pragma once

#include <Core/Assert.h>
#include <Core/Generic.h>

template<int N>
struct Axis
{
	Axis() = default;

	Axis(float min, float max) : lower(min), range(max - min)
	{
		ASSERT(max > min);
	}

	float CalculateAddress(float value) const
	{
		return Clamp(value - lower, 0.0f, range) * N / range;
	}

	float CalculateValue(int address) const
	{
		return lower + address * range / N;
	}

	// Valid range for input values
	//
	float lower = 0;
	float range = 1;
};

struct CountIterator
{
	CountIterator() = default;
	CountIterator(int value) : value(value) {}

	bool operator!=(const CountIterator& other) const
	{
		return value != other.value;
	}

	int operator*() const
	{
		return value;
	}

	const CountIterator& operator++()
	{
		++value;
		return *this;
	}

	int value = 0;
};

template<int N> CountIterator begin(Axis<N> axis) { unused(axis);  return CountIterator(0); }
template<int N> CountIterator end(Axis<N> axis) { unused(axis); return CountIterator(N + 1); }

template<typename T, int NI, int NJ = 1>
struct Lut
{
	Lut(float min, float max) : x(min, max)
	{
	}

	Lut(float xmin, float xmax, float ymin, float ymax) : x(xmin, xmax), y(ymin, ymax)
	{
	}

	T& operator[](int i)
	{
		ASSERT(i >= 0 && i <= NI);
		return values[i];
	}

	const T& operator[](int i) const
	{
		ASSERT(i >= 0 && i <= NI);
		return values[i];
	}

	T& operator()(int i, int j)
	{
		ASSERT(i >= 0 && i <= NI);
		ASSERT(j >= 0 && j <= NJ);
		return values[j * (NI + 1) + i];
	}

	T Sample(float s) const
	{
		const float address = x.CalculateAddress(s);

		const int i0 = int(address);
		const int i1 = Min(i0 + 1, NI);

		return Lerp(values[i0], values[i1], address - floorf(address));
	}

	T Sample(float s, float t) const
	{
		const float as = x.CalculateAddress(s);
		const float at = y.CalculateAddress(t);

		const int i0 = int(as);
		const int i1 = Min(i0 + 1, NI);
		const int j0 = int(at);
		const int j1 = Min(j0 + 1, NJ);

		const int c00 = j0 * (NI + 1) + i0;
		const int c01 = j0 * (NI + 1) + i1;
		const int c10 = j1 * (NI + 1) + i0;
		const int c11 = j1 * (NI + 1) + i1;

		const float fi = as - floorf(as);
		const float fj = at - floorf(at);

		return Lerp(Lerp(values[c00], values[c01], fi), Lerp(values[c10], values[c11], fi), fj);
	}

	// Values from min to max
	//
	T values[(NI + 1) * (NJ + 1)] = {};

	// Sample distribution for each axis
	//
	Axis<NI> x;
	Axis<NJ> y;
};