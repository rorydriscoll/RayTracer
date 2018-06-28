#pragma once

constexpr unsigned int ieee_positive_infinity = 0x7f800000;

constexpr float infinity = float(1e+300 * 1e+300);
constexpr float epsilon = 0.000001f;
constexpr float pi = 3.14159265358979f;
constexpr float invpi = 0.31830988618379f;
constexpr float tau = 6.283185307179586f;
constexpr float invtau = 0.159154943091895f;
constexpr float sqrt2 = 1.41421356f;
constexpr float invsqrt2 = 0.70710678f;
constexpr float max_float_value = 3.402823466e+38f;

const float deg_from_rad = 180 / pi;
const float rad_from_deg = pi / 180;

constexpr int max_int_value = 0x7fffffff;
constexpr int max_path_length = 255;

constexpr float operator""_deg(unsigned long long x) { return float(x * pi / 180); }
constexpr float operator""_deg(long double x) { return float(x * pi / 180); }

constexpr float operator""_mm(unsigned long long x) { return float(x / 1000.0); }
constexpr float operator""_mm(long double x) { return float(x / 1000000.0); }
constexpr float operator""_um(unsigned long long x) { return float(x / 1000.0); }
constexpr float operator""_um(long double x) { return float(x / 1000000.0); }

constexpr size_t operator""_KiB(unsigned long long x) { return size_t(1024 * x); }
constexpr size_t operator""_KiB(long double x) { return size_t(1024 * x); }
constexpr size_t operator""_MiB(unsigned long long x) { return size_t(1024 * 1024 * x); }
constexpr size_t operator""_MiB(long double x) { return size_t(1024 * 1024 * x); }
constexpr size_t operator""_GiB(unsigned long long x) { return size_t(1024 * 1024 * 1024 * x); }
constexpr size_t operator""_GiB(long double x) { return size_t(1024 * 1024 * 1024 * x); }
