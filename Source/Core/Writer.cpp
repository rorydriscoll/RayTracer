#include <Core/Writer.h>
#include <Core/Assert.h>
#include <cstring>
#include <cstdio>
#include <cstdarg>

template<typename T>
T Align(T value, size_t alignment)
{
	ASSERT((alignment & (alignment - 1)) == 0, "Alignment is not a power of two");

	return T(uintptr_t(value) + (alignment - 1) & ~(alignment - 1));
}

Writer::Writer(void* memory, size_t size) : 
	begin(static_cast<uint8_t*>(memory)), head(begin), end(head + size)
{
}

void Writer::Seek(size_t offset)
{
	head = begin + offset; 
	
	ASSERT(head <= end, "Writer seek to invalid offset");
}

void Writer::Skip(size_t offset)
{
	head += offset; 
	
	ASSERT(head <= end, "Writer skip to invalid offset");
}

void* Writer::Reserve(size_t size, size_t alignment)
{
	uint8_t* aligned = Align(head, alignment);

	ASSERT((aligned + size) <= end, "Writer is out of space");

	head = aligned + size;

	return reinterpret_cast<void*>(aligned);
}

void* Writer::Write(const void* value, size_t size, size_t alignment)
{
	void* memory = Reserve(size, alignment);

	memcpy(memory, value, size);

	return memory;
}

void Writer::Write(int8_t value)
{
	Write(&value, sizeof(int8_t), alignof(int8_t));
}

void Writer::Write(uint8_t value)
{
	Write(&value, sizeof(uint8_t), alignof(uint8_t));
}

void Writer::Write(int16_t value)
{
	Write(&value, sizeof(int16_t), alignof(int16_t));
}

void Writer::Write(uint16_t value)
{
	Write(&value, sizeof(uint16_t), alignof(uint16_t));
}

void Writer::Write(int32_t value)
{
	Write(&value, sizeof(int32_t), alignof(int32_t));
}

void Writer::Write(uint32_t value)
{
	Write(&value, sizeof(uint32_t), alignof(uint32_t));
}

void Writer::Write(uint64_t value)
{
	Write(&value, sizeof(uint64_t), alignof(uint64_t));
}

void Writer::Write(float value)
{
	Write(&value, sizeof(float), alignof(float));
}

void Writer::WriteString(const char* format, ...)
{
	char buffer[4096];

	va_list values;

	va_start(values, format);
	const size_t length = vsprintf_s(buffer, format, values);
	va_end(values);

	ASSERT(length <= size_t(end - head));

	memcpy(head, buffer, length);

	head += length;
}

size_t Writer::BytesWritten() const
{
	return head - begin;
}

size_t Writer::BytesRemaining() const
{
	return end - head;
}
