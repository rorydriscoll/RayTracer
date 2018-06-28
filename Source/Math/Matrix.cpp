#include <Math/Matrix.h>

float3x3 operator*(const float3x3& lhs, const float3x3& rhs)
{
	float3x3 result;

	result.xx = lhs.xx * rhs.xx + lhs.xy * rhs.yx + lhs.xz * rhs.zx;
	result.xy = lhs.xx * rhs.xy + lhs.xy * rhs.yy + lhs.xz * rhs.zy;
	result.xz = lhs.xx * rhs.xz + lhs.xy * rhs.yz + lhs.xz * rhs.zz;
	result.yx = lhs.yx * rhs.xx + lhs.yy * rhs.yx + lhs.yz * rhs.zx;
	result.yy = lhs.yx * rhs.xy + lhs.yy * rhs.yy + lhs.yz * rhs.zy;
	result.yz = lhs.yx * rhs.xz + lhs.yy * rhs.yz + lhs.yz * rhs.zz;
	result.zx = lhs.zx * rhs.xx + lhs.zy * rhs.yx + lhs.zz * rhs.zx;
	result.zy = lhs.zx * rhs.xy + lhs.zy * rhs.yy + lhs.zz * rhs.zy;
	result.zz = lhs.zx * rhs.xz + lhs.zy * rhs.yz + lhs.zz * rhs.zz;

	return result;
}

float4x4 operator*(const float4x4& lhs, const float4x4& rhs)
{
	float4x4 result;

	result.xx = lhs.xx * rhs.xx + lhs.xy * rhs.yx + lhs.xz * rhs.zx + lhs.xw * rhs.wx;
	result.xy = lhs.xx * rhs.xy + lhs.xy * rhs.yy + lhs.xz * rhs.zy + lhs.xw * rhs.wy;
	result.xz = lhs.xx * rhs.xz + lhs.xy * rhs.yz + lhs.xz * rhs.zz + lhs.xw * rhs.wz;
	result.xw = lhs.xx * rhs.xw + lhs.xy * rhs.yw + lhs.xz * rhs.zw + lhs.xw * rhs.ww;
	result.yx = lhs.yx * rhs.xx + lhs.yy * rhs.yx + lhs.yz * rhs.zx + lhs.yw * rhs.wx;
	result.yy = lhs.yx * rhs.xy + lhs.yy * rhs.yy + lhs.yz * rhs.zy + lhs.yw * rhs.wy;
	result.yz = lhs.yx * rhs.xz + lhs.yy * rhs.yz + lhs.yz * rhs.zz + lhs.yw * rhs.wz;
	result.yw = lhs.yx * rhs.xw + lhs.yy * rhs.yw + lhs.yz * rhs.zw + lhs.yw * rhs.ww;
	result.zx = lhs.zx * rhs.xx + lhs.zy * rhs.yx + lhs.zz * rhs.zx + lhs.zw * rhs.wx;
	result.zy = lhs.zx * rhs.xy + lhs.zy * rhs.yy + lhs.zz * rhs.zy + lhs.zw * rhs.wy;
	result.zz = lhs.zx * rhs.xz + lhs.zy * rhs.yz + lhs.zz * rhs.zz + lhs.zw * rhs.wz;
	result.zw = lhs.zx * rhs.xw + lhs.zy * rhs.yw + lhs.zz * rhs.zw + lhs.zw * rhs.ww;
	result.wx = lhs.wx * rhs.xx + lhs.wy * rhs.yx + lhs.wz * rhs.zx + lhs.ww * rhs.wx;
	result.wy = lhs.wx * rhs.xy + lhs.wy * rhs.yy + lhs.wz * rhs.zy + lhs.ww * rhs.wy;
	result.wz = lhs.wx * rhs.xz + lhs.wy * rhs.yz + lhs.wz * rhs.zz + lhs.ww * rhs.wz;
	result.ww = lhs.wx * rhs.xw + lhs.wy * rhs.yw + lhs.wz * rhs.zw + lhs.ww * rhs.ww;

	return result;
}

float4x4 operator*(const float4x4& lhs, const float3x4& rhs)
{
	float4x4 result;

	result.xx = lhs.xx * rhs.xx + lhs.xy * rhs.yx + lhs.xz * rhs.zx;
	result.xy = lhs.xx * rhs.xy + lhs.xy * rhs.yy + lhs.xz * rhs.zy;
	result.xz = lhs.xx * rhs.xz + lhs.xy * rhs.yz + lhs.xz * rhs.zz;
	result.xw = lhs.xx * rhs.xw + lhs.xy * rhs.yw + lhs.xz * rhs.zw + lhs.xw;
	result.yx = lhs.yx * rhs.xx + lhs.yy * rhs.yx + lhs.yz * rhs.zx;
	result.yy = lhs.yx * rhs.xy + lhs.yy * rhs.yy + lhs.yz * rhs.zy;
	result.yz = lhs.yx * rhs.xz + lhs.yy * rhs.yz + lhs.yz * rhs.zz;
	result.yw = lhs.yx * rhs.xw + lhs.yy * rhs.yw + lhs.yz * rhs.zw + lhs.yw;
	result.zx = lhs.zx * rhs.xx + lhs.zy * rhs.yx + lhs.zz * rhs.zx;
	result.zy = lhs.zx * rhs.xy + lhs.zy * rhs.yy + lhs.zz * rhs.zy;
	result.zz = lhs.zx * rhs.xz + lhs.zy * rhs.yz + lhs.zz * rhs.zz;
	result.zw = lhs.zx * rhs.xw + lhs.zy * rhs.yw + lhs.zz * rhs.zw + lhs.zw;
	result.wx = lhs.wx * rhs.xx + lhs.wy * rhs.yx + lhs.wz * rhs.zx;
	result.wy = lhs.wx * rhs.xy + lhs.wy * rhs.yy + lhs.wz * rhs.zy;
	result.wz = lhs.wx * rhs.xz + lhs.wy * rhs.yz + lhs.wz * rhs.zz;
	result.ww = lhs.wx * rhs.xw + lhs.wy * rhs.yw + lhs.wz * rhs.zw + lhs.ww;

	return result;
}

float2 operator*(const float2x2& lhs, float2 rhs)
{
	const float x = lhs.xx * rhs.x + lhs.xy * rhs.y;
	const float y = lhs.yx * rhs.x + lhs.yy * rhs.y;

	return {x, y};
}

float2 operator*(const float2x3& lhs, float3 rhs)
{
	const float x = lhs.xx * rhs.x + lhs.xy * rhs.y + lhs.xz * rhs.z;
	const float y = lhs.yx * rhs.x + lhs.yy * rhs.y + lhs.yz * rhs.z;

	return {x, y};
}

float3 operator*(const float3x3& lhs, float3 rhs)
{
	const float x = lhs.xx * rhs.x + lhs.xy * rhs.y + lhs.xz * rhs.z;
	const float y = lhs.yx * rhs.x + lhs.yy * rhs.y + lhs.yz * rhs.z;
	const float z = lhs.zx * rhs.x + lhs.zy * rhs.y + lhs.zz * rhs.z;

	return {x, y, z};
}

float3 operator*(const float3x4& lhs, float4 rhs)
{
	const float x = lhs.xx * rhs.x + lhs.xy * rhs.y + lhs.xz * rhs.z + lhs.xw * rhs.w;
	const float y = lhs.yx * rhs.x + lhs.yy * rhs.y + lhs.yz * rhs.z + lhs.yw * rhs.w;
	const float z = lhs.zx * rhs.x + lhs.zy * rhs.y + lhs.zz * rhs.z + lhs.zw * rhs.w;

	return {x, y, z};
}

float4 operator*(const float4x4& lhs, float4 rhs)
{
	const float x = lhs.xx * rhs.x + lhs.xy * rhs.y + lhs.xz * rhs.z + lhs.xw * rhs.w;
	const float y = lhs.yx * rhs.x + lhs.yy * rhs.y + lhs.yz * rhs.z + lhs.yw * rhs.w;
	const float z = lhs.zx * rhs.x + lhs.zy * rhs.y + lhs.zz * rhs.z + lhs.zw * rhs.w;
	const float w = lhs.wx * rhs.x + lhs.wy * rhs.y + lhs.wz * rhs.z + lhs.ww * rhs.w;

	return {x, y, z, w};
}

float3 ProjectPoint(const float4x4& lhs, float3 rhs)
{
	const float x = lhs.xx * rhs.x + lhs.xy * rhs.y + lhs.xz * rhs.z + lhs.xw;
	const float y = lhs.yx * rhs.x + lhs.yy * rhs.y + lhs.yz * rhs.z + lhs.yw;
	const float z = lhs.zx * rhs.x + lhs.zy * rhs.y + lhs.zz * rhs.z + lhs.zw;
	const float w = lhs.wx * rhs.x + lhs.wy * rhs.y + lhs.wz * rhs.z + lhs.ww;

	return {x / w, y / w, z / w};
}

float4x4 CalculatePerspectiveProjection(float hfov, float vfov, float near)
{
	const float n = near;
	const float w = 1 / Tan(hfov * 0.5f);
	const float h = w * hfov / vfov;

	return {w, 0, 0, 0, 0, h, 0, 0, 0, 0, 1, -n, 0, 0, 1, 0};
}

float4x4 CalculateInversePerspectiveProjection(float hfov, float vfov, float near)
{
	const float n = near;
	const float w = Tan(hfov * 0.5f);
	const float h = w * vfov / hfov;

	return {w, 0, 0, 0, 0, h, 0, 0, 0, 0, 0, 1, 0, 0, -1 / n, +1 / n};
}

float4x4 CalculatePerspectiveProjection(float hfov, float vfov, float near, float far)
{
	const float n = near;
	const float f = far;
	const float q = f / (f - n);
	const float w = 1 / Tan(hfov * 0.5f);
	const float h = w * hfov / vfov;

	return {w, 0, 0, 0, 0, h, 0, 0, 0, 0, q, -q * n, 0, 0, 1, 0};
}

float4x4 CalculateInversePerspectiveProjection(float hfov, float vfov, float near, float far)
{
	const float n = near;
	const float f = far;
	const float q = f / (f - n);
	const float w = Tan(hfov * 0.5f);
	const float h = w * vfov / hfov;

	return {w, 0, 0, 0, 0, h, 0, 0, 0, 0, 0, 1, 0, 0, -1 / (q * n), +1 / n};
}

float4x4 CalculateOrthographicProjection(float w, float h, float d)
{
	return {2 / w, 0, 0, 0, 0, 2 / h, 0, 0, 0, 0, 1 / d, 0, 0, 0, 0, 1};
}

float4x4 CalculateInverseOrthographicProjection(float w, float h, float d)
{
	return {w / 2, 0, 0, 0, 0, h / 2, 0, 0, 0, 0, d, 0, 0, 0, 0, 1};
}

float4x4 CalculateScreenFromClipMatrix(uint w, uint h)
{
	const float hw = w * 0.5f;
	const float hh = h * 0.5f;

	return {hw, 0, 0, hw, 0, -hh, 0, hh, 0, 0, 1, 0, 0, 0, 0, 1};
}

float4x4 CalculateClipFromScreenMatrix(uint w, uint h)
{
	const float hw = w * 0.5f;
	const float hh = h * 0.5f;

	return {1 / hw, 0, 0, -1, 0, -1 / hh, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1};
}
