#include <Core/Assert.h>
#include <Core/Log.h>
#include <UnitTest++/AssertException.h>
#include <cstdio>
#include <cstdarg>

namespace
{
	// For unit tests we want to throw exceptions rather than halting
	//
	bool throw_exceptions = false;
}

void Assert::ThrowExceptions()
{
	throw_exceptions = true;
}

void Assert::OnConditionFailed(const char* condition, const char* filename, int line)
{
	LOG_ERROR("\n%s(%i): %s\n", filename, line, condition);

	if (throw_exceptions)
		throw UnitTest::AssertException(condition, filename, line);
}

void Assert::OnConditionFailed(const char* condition, const char* filename, int line, const char* message, ...)
{
	char buffer[1024];
		
	va_list args;

	va_start(args, message);
	vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, message, args);
	va_end(args);

	LOG_ERROR("\n%s(%i): %s (%s)\n", filename , line , buffer, condition);

	if (throw_exceptions)
		throw UnitTest::AssertException(buffer, filename, line);
}

void Assert::ReportFatalError(const char* filename, int line, const char* message, ...)
{
	char buffer[1024];

	va_list args;

	va_start(args, message);
	vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, message, args);
	va_end(args);

	LOG_ERROR("\n%s(%i): %s\n", filename , line , buffer);

	if (throw_exceptions)
		throw UnitTest::AssertException(buffer, filename, line);
}
