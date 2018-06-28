#pragma once

namespace Assert
{
	void ThrowExceptions();
	void OnConditionFailed(const char* condition, const char* filename, int line);
	void OnConditionFailed(const char* condition, const char* filename, int line, const char* message, ...);
	void ReportFatalError(const char* filename, int line, const char* message, ...);
}

#define HALT() __debugbreak()

#ifndef FINAL_BUILD

#define ASSERT(CONDITION__, ...) \
	do \
	{ \
		if (!(CONDITION__)) \
		{ \
			Assert::OnConditionFailed(#CONDITION__, __FILE__, __LINE__, ##__VA_ARGS__); \
			HALT(); \
		} \
	} while(0)

#define CRITICAL(CONDITION__, MESSAGE__, ...) \
	do \
	{ \
		if (!(CONDITION__)) \
		{ \
			Assert::ReportFatalError(__FILE__, __LINE__, MESSAGE__, ##__VA_ARGS__); \
			HALT(); \
		} \
	} while(0)

#else

#define ASSERT(CONDITION__, ...)

#define CRITICAL(CONDITION__, MESSAGE__, ...) \
	do \
	{ \
	if (!(CONDITION__)) \
		{ \
		Assert::OnConditionFailed(#CONDITION__, __FILE__, __LINE__, MESSAGE__, ##__VA_ARGS__); \
		HALT(); \
		} \
	} while(0)
#define COMPILE_ASSERT(x)

#endif
