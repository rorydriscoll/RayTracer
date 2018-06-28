#pragma once

#include <cmath>

inline float Min(float a, float b, float c)
{
	return a < b ? a < c ? a : c : b < c ? b : c;
}

inline float Max(float a, float b, float c)
{
	return a > b ? a > c ? a : c : b > c ? b : c;
}

inline float Floor(float f)
{
	return floorf(f);
}

inline float Fract(float f)
{
	return f - int(f);
}

inline float Abs(float x)
{
	return x < 0 ? -x : +x;
}

inline float Mod(float x, float y)
{
	return fmodf(x, y);
}

inline float Wrap(float x, float y)
{
	return x - y * floor(x / y);
}

inline float Sqrt(float x)
{
	return sqrtf(x);
}

inline float Pow(float x, float y)
{
	return powf(x, y);
}

inline float Log2(float x)
{
	return log2f(x);
}

inline float Exp(float x)
{
	return expf(x);
}

inline float Ln(float x)
{
	return logf(x);
}

inline float Square(float x)
{
	return x * x;
}

inline float Saturate(float v)
{
	return v < 0 ? 0 : v > 1 ? 1 : v;
}

inline float Sin(float x)
{
	return sinf(x);
}

inline float ASin(float x)
{
	return asinf(x);
}

inline float Cos(float x)
{
	return cosf(x);
}

inline float ACos(float x)
{
	return acosf(x);
}

inline float Tan(float x)
{
	return tanf(x);
}

inline float ATan(float x)
{
	return atanf(x);
}

inline float ATan(float x, float y)
{
	return atan2f(x, y);
}

inline float Sign(float x)
{
	return x >= 0 ? 1.0f : -1.0f;
}

inline float GammaToLinear(float gamma)
{
	return gamma <= 0.04045f ? (gamma / 12.92f) : (powf((gamma + 0.055f) / 1.055f, 2.4f));
}

inline float LinearToGamma(float linear)
{
	return linear <= 0.0031308f ? (linear * 12.92f) : (1.055f * powf(linear, 1.0f / 2.4f) - 0.055f);
}

inline bool IsNan(float v)
{
	return isnan(v);
}

inline bool IsInf(float v)
{
	return isinf(v);
}

inline int Square(int x)
{
	return x * x;
}