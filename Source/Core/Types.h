#pragma once

using int8_t = signed char;
using uint8_t = unsigned char;
using int16_t = signed short;
using uint16_t = unsigned short;
using int32_t = signed int;
using uint32_t = unsigned int;
using int64_t = signed long long;
using uint64_t = unsigned long long;

#ifdef _WIN64

using size_t = unsigned long long;
using ptr_t = unsigned long long;
using ptrdiff_t = signed long long;

#else

using size_t = unsigned int;
using ptr_t = unsigned int;
using ptrdiff_t = signed int;

#endif

using uint = unsigned int;

#define countof(X) int(sizeof(X) / sizeof(X[0]))
#define unused(X) (void)X