#pragma once

#include <Math/Vector.h>
#include <Core/Types.h>

template<typename T, int Rows, int Cols>
struct Matrix
{
};

template<>
struct Matrix<float, 2, 2>
{
	Matrix() = default;
	Matrix(float xx, float xy, float yx, float yy) : xx(xx), xy(xy), yx(yx), yy(yy) {}

	union
	{
		float2 row[2];
		struct { float xx, xy, yx, yy; };
	};
};

template<>
struct Matrix<float, 2, 3>
{
	Matrix() = default;
	Matrix(float xx, float xy, float xz, float yx, float yy, float yz) : xx(xx), xy(xy), xz(xz), yx(yx), yy(yy), yz(yz) {}

	union
	{
		float3 row[2];
		struct { float xx, xy, xz, yx, yy, yz; };
	};
};

template<>
struct Matrix<float, 3, 3>
{
	Matrix() = default;
	Matrix(float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz) :
		xx(xx), xy(xy), xz(xz), yx(yx), yy(yy), yz(yz), zx(zx), zy(zy), zz(zz) {}
	Matrix(float3 x, float3 y, float3 z) :
		xx(x.x), xy(y.x), xz(z.x), yx(x.y), yy(y.y), yz(z.y), zx(x.z), zy(y.z), zz(z.z) {}

	union
	{
		float3 row[3];
		struct { float xx, xy, xz, yx, yy, yz, zx, zy, zz; };
	};
};

template<>
struct Matrix<float, 3, 4>
{
	Matrix() = default;
	Matrix(float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw, float zx, float zy, float zz, float zw) :
		xx(xx), xy(xy), xz(xz), xw(xw), yx(yx), yy(yy), yz(yz), yw(yw), zx(zx), zy(zy), zz(zz), zw(zw) {}

	union
	{
		float4 row[3];
		struct { float xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw; };
	};
};

template<>
struct Matrix<float, 4, 4>
{
	Matrix() = default;
	Matrix(float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw, float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww) :
		xx(xx), xy(xy), xz(xz), xw(xw), yx(yx), yy(yy), yz(yz), yw(yw), zx(zx), zy(zy), zz(zz), zw(zw), wx(wx), wy(wy), wz(wz), ww(ww) {}

	union
	{
		float4 row[4];
		struct { float xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw, wx, wy, wz, ww; };
	};
};

using float2x2 = Matrix<float, 2, 2>;
using float2x3 = Matrix<float, 2, 3>;
using float3x3 = Matrix<float, 3, 3>;
using float3x4 = Matrix<float, 3, 4>;
using float4x4 = Matrix<float, 4, 4>;

// Matrix-matrix operators
//
float3x3 operator*(const float3x3& lhs, const float3x3& rhs);
float4x4 operator*(const float4x4& lhs, const float4x4& rhs);
float4x4 operator*(const float4x4& lhs, const float3x4& rhs);

// Transform a vector
//
float2 operator*(const float2x2& lhs, float2 rhs);
float2 operator*(const float2x3& lhs, float3 rhs);
float3 operator*(const float3x3& lhs, float3 rhs);
float3 operator*(const float3x4& lhs, float4 rhs);
float4 operator*(const float4x4& lhs, float4 rhs);

// Transpose
//
inline float2x2 Transpose(float2x2 m) { return {m.xx, m.yx, m.xy, m.yy}; }
inline float3x3 Transpose(float3x3 m) { return {m.xx, m.yx, m.zx, m.xy, m.yy, m.zy, m.xz, m.yz, m.zz}; }

// Project a point through the matrix
//
float3 ProjectPoint(const float4x4& lhs, float3 rhs);

// Projections
//
float4x4 CalculatePerspectiveProjection(float hfov, float vfov, float near);
float4x4 CalculatePerspectiveProjection(float hfov, float vfov, float near, float far);
float4x4 CalculateInversePerspectiveProjection(float hfov, float vfov, float near);
float4x4 CalculateInversePerspectiveProjection(float hfov, float vfov, float near, float far);
float4x4 CalculateOrthographicProjection(float w, float h, float d);
float4x4 CalculateInverseOrthographicProjection(float w, float h, float d);
float4x4 CalculateScreenFromClipMatrix(uint w, uint h);
float4x4 CalculateClipFromScreenMatrix(uint w, uint h);

// Rotations

inline float3x3 MakeRotationMatrix(float3 y)
{
	const float3 ref = Abs(Dot(y, float3(0, 1, 0))) > 0.99f ? float3(0, 0, 1) : float3(0, 1, 0);

	const float3 x = Normalize(Cross(y, ref));
	const float3 z = Cross(x, y);

	return { x, y, z };
}

inline float3x3 MakeRotationMatrix2(float3 z)
{
	const float3 ref = Abs(Dot(z, float3(0, 1, 0))) > 0.99f ? float3(0, 0, 1) : float3(0, 1, 0);

	const float3 x = Normalize(Cross(ref, z));
	const float3 y = Cross(z, x);

	return { x, y, z };
}
