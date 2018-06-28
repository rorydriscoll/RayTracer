#pragma once

#include <Math/Scalar.h>
#include <Core/Types.h>

template<typename T, int Count>
struct Vector
{
	T values[Count];
};

template<typename T>
struct Vector<T, 2>
{
	Vector() = default;
	explicit Vector(T xy) : x(xy), y(xy) {}
	template<typename R> explicit Vector(Vector<R, 2> xy) : x(T(xy.x)), y(T(xy.y)) {}
	Vector(T x, T y) : x(x), y(y) {}

	union
	{
		struct { T x, y; };
		T values[2];
	};
};

template<typename T>
struct Vector<T, 3>
{
	Vector() = default;
	explicit Vector(T xyz) : x(xyz), y(xyz), z(xyz) {}
	Vector(Vector<T, 2> xy, T z) : x(xy.x), y(xy.y), z(z) {}
	Vector(T x, T y, T z) : x(x), y(y), z(z) {}

	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
		T values[3];
	};
};

template<typename T>
struct Vector<T, 4>
{
	Vector() = default;
	explicit Vector(T xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
	Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
	Vector(Vector<T, 3> xyz, T w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		T values[4];
	};
};

using int1 = int;
using int2 = Vector<int, 2>;
using int3 = Vector<int, 3>;
using int4 = Vector<int, 4>;

using float1 = float;
using float2 = Vector<float, 2>;
using float3 = Vector<float, 3>;
using float4 = Vector<float, 4>;

//
// Generic vector 2
//

template<typename T> Vector<T, 2> operator+(Vector<T, 2> v)
{
	return v;
}

template<typename T> Vector<T, 2> operator-(Vector<T, 2> s)
{
	return { -s.x, -s.y };
}

template<typename T> Vector<T, 2> operator+(Vector<T, 2> lhs, Vector<T, 2> rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

template<typename T> Vector<T, 2> operator-(Vector<T, 2> lhs, Vector<T, 2> rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

template<typename T> Vector<T, 2> operator*(Vector<T, 2> lhs, float rhs)
{
	return { lhs.x * rhs, lhs.y * rhs };
}

template<typename T> Vector<T, 2> operator*(float lhs, Vector<T, 2> rhs)
{
	return { lhs * rhs.x, lhs * rhs.y };
}

template<typename T> Vector<T, 2> operator*(Vector<T, 2> lhs, int rhs)
{
	return { lhs.x * rhs, lhs.y * rhs };
}

template<typename T> Vector<T, 2> operator*(int lhs, Vector<T, 2> rhs)
{
	return { lhs * rhs.x, lhs * rhs.y };
}

template<typename T> Vector<T, 2> operator*(Vector<T, 2> lhs, Vector<T, 2> rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}

template<typename T> Vector<T, 2> operator/(Vector<T, 2> lhs, float rhs)
{
	return { lhs.x / rhs, lhs.y / rhs };
}

template<typename T> Vector<T, 2> operator/(float lhs, Vector<T, 2> rhs)
{
	return { lhs / rhs.x, lhs / rhs.y };
}

template<typename T> Vector<T, 2> operator/(Vector<T, 2> lhs, Vector<T, 2> rhs)
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

template<typename T> Vector<T, 2>& operator+=(Vector<T, 2>& lhs, Vector<T, 2> rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;

	return lhs;
}

template<typename T> Vector<T, 2>& operator-=(Vector<T, 2>& lhs, Vector<T, 2> rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;

	return lhs;
}

template<typename T> Vector<T, 2>& operator*=(Vector<T, 2>& lhs, float rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;

	return lhs;
}

template<typename T> Vector<T, 2>& operator/=(Vector<T, 2>& lhs, float rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;

	return lhs;
}

template<typename T> Vector<T, 2> Min(Vector<T, 2> a, Vector<T, 2> b)
{
	return { Min(a.x, b.x), Min(a.y, b.y) };
}

template<typename T> Vector<T, 2> Max(Vector<T, 2> a, Vector<T, 2> b)
{
	return { Max(a.x, b.x), Max(a.y, b.y) };
}

template<typename T> Vector<T, 2> Clamp(Vector<T, 2> v, Vector<T, 2> min, Vector<T, 2> max)
{
	return { Clamp(v.x, min.x, max.x), Clamp(v.y, min.y, max.y) };
}

template<typename T> T Dot(Vector<T, 2> lhs, Vector<T, 2> rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename T> T Dot(Vector<T, 2> v)
{
	return v.x * v.x + v.y * v.y;
}

template<typename T> Vector<T, 2> Perpendicular(Vector<T, 2> v)
{
	return { -v.y, v.x };
}

//
// Generic vector 3
//

template<typename T> Vector<T, 3> operator+(Vector<T, 3> v)
{
	return v;
}

template<typename T> Vector<T, 3> operator-(Vector<T, 3> s)
{
	return { -s.x, -s.y, -s.z };
}

template<typename T> Vector<T, 3> operator+(Vector<T, 3> lhs, Vector<T, 3> rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template<typename T> Vector<T, 3> operator-(Vector<T, 3> lhs, Vector<T, 3> rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

template<typename T> Vector<T, 3> operator-(float lhs, Vector<T, 3> rhs)
{
	return { lhs - rhs.x, lhs - rhs.y, lhs - rhs.z };
}

template<typename T> Vector<T, 3> operator*(Vector<T, 3> lhs, Vector<T, 3> rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

template<typename T> Vector<T, 3> operator*(Vector<T, 3> lhs, float rhs)
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

template<typename T> Vector<T, 3> operator*(float lhs, Vector<T, 3> rhs)
{
	return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
}

template<typename T> Vector<T, 3> operator/(Vector<T, 3> lhs, Vector<T, 3> rhs)
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
}

template<typename T> Vector<T, 3> operator/(Vector<T, 3> lhs, float rhs)
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
}

template<typename T> Vector<T, 3> operator/(float lhs, Vector<T, 3> rhs)
{
	return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z };
}

template<typename T> Vector<T, 3>& operator+=(Vector<T, 3>& lhs, Vector<T, 3> rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;

	return lhs;
}

template<typename T> Vector<T, 3>& operator-=(Vector<T, 3>& lhs, Vector<T, 3> rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;

	return lhs;
}

template<typename T> Vector<T, 3>& operator*=(Vector<T, 3>& lhs, Vector<T, 3> rhs)
{
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;

	return lhs;
}

template<typename T> Vector<T, 3>& operator/=(Vector<T, 3>& lhs, Vector<T, 3> rhs)
{
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;

	return lhs;
}
template<typename T> Vector<T, 3>& operator*=(Vector<T, 3>& lhs, float rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;

	return lhs;
}

template<typename T> Vector<T, 3>& operator/=(Vector<T, 3>& lhs, float rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;

	return lhs;
}

template<typename T> Vector<T, 3> Min(Vector<T, 3> a, Vector<T, 3> b)
{
	return { Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z) };
}

template<typename T> Vector<T, 3> Max(Vector<T, 3> a, Vector<T, 3> b)
{
	return { Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z) };
}

template<typename T> float Dot(Vector<T, 3> v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

template<typename T> float Dot(Vector<T, 3> lhs, Vector<T, 3> rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

//
// Generic vector 4
//

template<typename T> Vector<T, 4> operator+(Vector<T, 4> v)
{
	return v;
}

template<typename T> Vector<T, 4> operator-(Vector<T, 4> s)
{
	return { -s.x, -s.y, -s.z, -s.w };
}

template<typename T> Vector<T, 4> operator+(Vector<T, 4> lhs, Vector<T, 4> rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

template<typename T> Vector<T, 4> operator-(Vector<T, 4> lhs, Vector<T, 4> rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

template<typename T> Vector<T, 4> operator-(float lhs, Vector<T, 4> rhs)
{
	return { lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w };
}

template<typename T> Vector<T, 4> operator*(Vector<T, 4> lhs, Vector<T, 4> rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
}

template<typename T> Vector<T, 4> operator*(Vector<T, 4> lhs, float rhs)
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
}

template<typename T> Vector<T, 4> operator*(float lhs, Vector<T, 4> rhs)
{
	return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w };
}

template<typename T> Vector<T, 4> operator/(Vector<T, 4> lhs, Vector<T, 4> rhs)
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w };
}

template<typename T> Vector<T, 4> operator/(Vector<T, 4> lhs, float rhs)
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
}

template<typename T> Vector<T, 4> operator/(float lhs, Vector<T, 4> rhs)
{
	return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w };
}

template<typename T> Vector<T, 4>& operator+=(Vector<T, 4>& lhs, Vector<T, 4> rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;

	return lhs;
}

template<typename T> Vector<T, 4>& operator-=(Vector<T, 4>& lhs, Vector<T, 4> rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	lhs.w -= rhs.w;

	return lhs;
}

template<typename T> Vector<T, 4>& operator*=(Vector<T, 4>& lhs, float rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	lhs.w *= rhs;

	return lhs;
}

template<typename T> Vector<T, 4>& operator/=(Vector<T, 4>& lhs, float rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	lhs.w /= rhs;

	return lhs;
}

template<typename T> float Dot(Vector<T, 4> a, Vector<T, 4> b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

//
// float2
//

inline float Cross(float2 lhs, float2 rhs)
{
	// Same as Dot(Perpendicular(lhs), rhs)

	return lhs.x * rhs.y - lhs.y * rhs.x;
}

inline float Length(float2 v)
{
	return Sqrt(v.x * v.x + v.y * v.y);
}

inline float2 Normalize(float2 v)
{
	return v * (1.0f / Length(v));
}

inline float2 Floor(float2 v)
{
	return { floorf(v.x), floorf(v.y) };
}

//
// float3
//

inline float3 Reflect(float3 v, float3 n)
{
	return v - 2 * Dot(n, v) * n;
}

inline float3 Exp(float3 v)
{
	return { exp(v.x), exp(v.y), exp(v.z) };
}

inline float3 Sqrt(float3 v)
{
	return { sqrt(v.x), sqrt(v.y), sqrt(v.z) };
}

inline float3 Abs(float3 v)
{
	return { Abs(v.x), Abs(v.y), Abs(v.z) };
}

inline float3 Cross(float3 lhs, float3 rhs)
{
	const float x = lhs.y * rhs.z - lhs.z * rhs.y;
	const float y = lhs.z * rhs.x - lhs.x * rhs.z;
	const float z = lhs.x * rhs.y - lhs.y * rhs.x;

	return { x, y, z };
}

inline float Length(float3 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float3 Sign(float3 v)
{
	return { Sign(v.x), Sign(v.y), Sign(v.z) };
}

inline float3 Normalize(float3 v)
{
	return v / Length(v);
}

inline float3 Normalize(float3 v, float3 fallback)
{
	const float l = Length(v);

	return l == 0 ? fallback : v / l;
}

inline float3 Floor(float3 v)
{
	return { floorf(v.x), floorf(v.y), floorf(v.z) };
}

inline float3 Saturate(float3 v)
{
	return { Saturate(v.x), Saturate(v.y), Saturate(v.z) };
}

inline float3 LinearToGamma(float3 v)
{
	return { LinearToGamma(v.r), LinearToGamma(v.g), LinearToGamma(v.b) };
}

inline bool ContainsNan(float3 v)
{
	return IsNan(v.x) || IsNan(v.y) || IsNan(v.z);
}

inline bool ContainsInf(float3 v)
{
	return IsInf(v.x) || IsInf(v.y) || IsInf(v.z);
}

//
// float4
//

inline float Length(float4 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

inline float4 Normalize(float4 v)
{
	return v / Length(v);
}

//
// int2 
//

inline bool operator==(int2 lhs, int2 rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(int2 lhs, int2 rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y;
}

inline int2 RoundDown(float2 v)
{
	return { int(floorf(v.x)), int(floorf(v.y)) };
}

//
// int3
//

inline bool operator==(int3 lhs, int3 rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline bool operator!=(int3 lhs, int3 rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

//
// int4
//

inline bool operator==(int4 lhs, int4 rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

inline bool operator!=(int4 lhs, int4 rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
}

//
// color
//

inline float CalculateLuminance(float3 color)
{
	return 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
}

inline float3 Rgb(uint8_t r, uint8_t g, uint8_t b)
{
	return { GammaToLinear(r / 255.0f), GammaToLinear(g / 255.0f), GammaToLinear(b / 255.0f) };
}

inline float3 Rgb(uint32_t rgb)
{
	const uint8_t r = (rgb >> 16) & 0xff;
	const uint8_t g = (rgb >> 8) & 0xff;
	const uint8_t b = (rgb >> 0) & 0xff;

	return Rgb(r, g, b);
}
