#pragma once

#include <utility>

template<typename T> T Min(T a, T b)
{
	return a < b ? a : b;
}

template<typename T> T Max(T a, T b)
{
	return a > b ? a : b;
}

template<typename T> T Clamp(T v, T min, T max)
{
	return v > max ? max : v < min ? min : v;
}

template<typename T> T Lerp(T a, T b, float t)
{
	return a * (1 - t) + b * t;
}

template<typename T> void Swap(T& a, T& b)
{
	T temp = std::move(a);

	a = std::move(b);
	b = std::move(temp);
}
