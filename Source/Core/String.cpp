#include <Core/String.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>

size_t String::Format(char* buffer, size_t size, const char* format, ...)
{
	va_list values;

	va_start(values, format);
	const int formatted_size = vsnprintf_s(buffer, size, _TRUNCATE, format, values);
	va_end(values);

	return formatted_size < 0 ? size : formatted_size;
}

void String::Copy(char* target, const char* source, size_t count)
{
	strncpy(target, source, count);
}